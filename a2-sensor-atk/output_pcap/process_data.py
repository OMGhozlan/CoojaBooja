#!/usr/bin/env python

import sys
import numpy as np
import pandas as pd


columns = ['Session Key', 'Protocol', 'Src IP', 'Src Port', 
'Dst IP', 'Dst Port', 'Init Ts', 'Last Ts', 'Ts', 'Duration', 'Mac Addr', 'Label']

def cat_files():
    df = []
    for i in range(1, 4):
        df.append(pd.read_csv(f'a2_{i}_labeled.csv'))

    df = pd.concat(df)
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