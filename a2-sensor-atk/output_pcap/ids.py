import sys
import pandas as pd
import numpy as np
from sklearn.naive_bayes import ComplementNB, GaussianNB, MultinomialNB, BernoulliNB, CategoricalNB
from sklearn.ensemble import RandomForestClassifier, AdaBoostClassifier
from sklearn.model_selection import KFold, cross_val_score, cross_validate
from sklearn.metrics import (classification_report, confusion_matrix,
                             plot_confusion_matrix,
                             precision_recall_fscore_support)


if __name__ == '__main__':

       kf = KFold(n_splits=10, shuffle=True)
       
       file_ = None
       if len(sys.argv) == 1:
              while not file_:
                     file_ = input("Enter file name:")
       else:
              file_ = sys.argv[1]

       df = pd.read_csv(file_)

       features = list(df.columns)
       features.remove('label')

       X = df[features]
       y = df['label']

       clfs = [AdaBoostClassifier(n_estimators=100), RandomForestClassifier(n_estimators=100), GaussianNB(), MultinomialNB(), BernoulliNB()] #, ComplementNB()]

       cv_results = []

       for clf in clfs:
              print(clf)
              cv_results.append(pd.DataFrame(cross_validate(clf, X, y, cv=kf, scoring=('accuracy', 'f1', 'precision', 'recall'))))
       
       print(cv_results)
