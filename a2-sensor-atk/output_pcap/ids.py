import pandas as pd
import numpy as np
import netaddr
import re
from datetime import datetime
from sklearn.naive_bayes import ComplementNB, GaussianNB, MultinomialNB, BernoulliNB, CategoricalNB
from sklearn.ensemble import RandomForestClassifier, AdaBoostClassifier
from sklearn.model_selection import KFold, cross_val_score, cross_validate
from sklearn.metrics import (classification_report, confusion_matrix,
                             plot_confusion_matrix,
                             precision_recall_fscore_support)


def timestamp(dt):
    zero = datetime(1900, 1, 1, 0, 0, 0, 0)
    dt = datetime.strptime(dt,'%M:%S.%f')
    return (dt - zero).total_seconds() * 1000.0


x_columns = ['icmpv6', 'udp', 'wpan', 'src_mac', 'src_ip', 'src_port', 'dst_mac', 'dst_ip', 'dst_port',
       'init_ts', 'last_ts', 'ts', 'duration']

# Load data
df = pd.read_csv("data.csv")

dst_mac = []
src_mac = []
for i in range(len(df)):
       macs = df['Mac Addr'][i]
       if macs == 'set()':
              m = [[0, 0], [0, 0]]
       else:       
              m = re.findall('(([0-9A-Fa-f]{2}[:-]){7}[0-9A-Fa-f]{2})', df['Mac Addr'][i])
       dst_mac.append(int(netaddr.EUI(m[0][0])))
       src_mac.append(int(netaddr.EUI(m[1][0])))

df = pd.DataFrame({'src_mac': src_mac, 'dst_mac': dst_mac}).join(df)
del dst_mac, src_mac

# Perform one-hot encoding on protocols used
df = pd.get_dummies(df['Protocol']).join(df)

# Drop the 'protocol' column
df.drop('Protocol', axis=1, inplace=True)


# Drop the 'mac_addr' column
df.drop('Mac Addr', axis=1, inplace=True)

# Turn label into numerical (0: Normal, 1: Malicious)
df['Label'] = (df['Label'] == 'Malicious').astype(int)

# Renaming columns (ease of use)
df.columns = [col.lower().replace(" ", "_") for col in df.columns]

# Convert IP address into int to be fed into classifier
df['src_ip'] = df['src_ip'].apply(lambda x: netaddr.IPAddress(x) if x != '0' else 0)
df['dst_ip'] = df['dst_ip'].apply(lambda x: netaddr.IPAddress(x) if x != '0' else 0)

# Convert times into milliseconds to be fed into classifier
df['init_ts'] = df['init_ts'].apply(lambda x: timestamp(x))
df['last_ts'] = df['last_ts'].apply(lambda x: timestamp(x))


df = df[['icmpv6', 'udp', 'wpan', 'src_mac', 'src_ip', 'src_port', 'dst_mac', 'dst_ip', 'dst_port',
       'init_ts', 'last_ts', 'ts', 'duration', 'label']]
df.to_csv('data_processed.csv', index=False)

X = df[x_columns]
y = df['label']

kf = KFold(n_splits=10, shuffle=True)

clfs = [AdaBoostClassifier(n_estimators=100), RandomForestClassifier(n_estimators=100), GaussianNB(), MultinomialNB(), BernoulliNB()] #, ComplementNB()]

cv_results = []

for clf in clfs:
    print(clf)
    cv_results.append(pd.DataFrame(cross_validate(clf, X, y, cv=kf, scoring=('accuracy', 'f1', 'precision', 'recall'))))
    
print(cv_results)
