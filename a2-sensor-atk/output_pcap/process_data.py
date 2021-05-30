#!/usr/bin/env python

import sys
import numpy as np
import pandas as pd
import netaddr
import re
from datetime import datetime


columns = ['session_key', 'protocol', 'src_ip', 'src_port','dst_ip', 'dst_port', 
'init_ts', 'last_ts', 'ts', 'duration', 'mac_addr','label']


x_columns = ['icmpv6', 'udp', 'wpan', 'src_mac', 'src_ip', 'src_port', 'dst_mac', 'dst_ip', 'dst_port',
       'init_ts', 'last_ts', 'ts', 'duration']

def timestamp(dt):
    zero = datetime(1900, 1, 1, 0, 0, 0, 0)
    dt = datetime.strptime(dt,'%M:%S.%f')
    return (dt - zero).total_seconds() * 1000.0


def process_other_feats(file="data.csv", to_csv=True):
       # Load data
       df = pd.read_csv(file)

       dst_mac = []
       src_mac = []
       for i in range(len(df)):
              macs = df['mac_addr'][i]
              if macs == 'set()':
                     m = [[0, 0], [0, 0]]
              else:       
                     m = re.findall('(([0-9A-Fa-f]{2}[:-]){7}[0-9A-Fa-f]{2})', df['mac_addr'][i])
              dst_mac.append(int(netaddr.EUI(m[0][0])))
              src_mac.append(int(netaddr.EUI(m[1][0])))

       df = pd.DataFrame({'src_mac': src_mac, 'dst_mac': dst_mac}).join(df)
       del dst_mac, src_mac

       df['protocol'] = df['protocol'].apply(lambda x: x.lower())

       # Perform one-hot encoding on protocols used
       df = pd.get_dummies(df['protocol']).join(df)
       print(df.head())

       # Drop the 'protocol' column
       df.drop('protocol', axis=1, inplace=True)

       # Drop the 'mac_addr' column
       df.drop('mac_addr', axis=1, inplace=True)

       # Turn label into numerical (0: Normal, 1: Malicious)
       df['label'] = (df['label'] == 'Malicious').astype(int)

       # Convert IP address into int to be fed into classifier
       df['src_ip'] = df['src_ip'].apply(lambda x: int(netaddr.IPAddress(x)) if x != '0' else 0)
       df['dst_ip'] = df['dst_ip'].apply(lambda x: int(netaddr.IPAddress(x)) if x != '0' else 0)

       # Convert times into milliseconds to be fed into classifier
       df['init_ts'] = df['init_ts'].apply(lambda x: timestamp(x))
       df['last_ts'] = df['last_ts'].apply(lambda x: timestamp(x))


       df = df[['icmpv6', 'udp', 'wpan', 'src_mac', 'src_ip', 'src_port', 'dst_mac', 'dst_ip', 'dst_port',
              'init_ts', 'last_ts', 'ts', 'duration', 'label']]
       if to_csv:
              df.to_csv('data_processed.csv', index=False)
       return df

def cat_files():
    df = []
    for i in range(1, 4):
        df.append(pd.read_csv(f'a2_{i}_labeled.csv'))

    df = pd.concat(df)
    df.columns = [col.lower().replace(" ", "_") for col in df.columns]
    return df


if __name__ == '__main__':
    df = cat_files()
    if len(sys.argv) == 2:
        df[columns[1:]].to_csv(f'data.csv', index=False)
    else:
        df.drop(columns=columns[:-1], inplace=True)
        df.fillna(0, inplace=True)
        df.to_csv(f'data_l.csv', index=False)
    print("[*] Done!")