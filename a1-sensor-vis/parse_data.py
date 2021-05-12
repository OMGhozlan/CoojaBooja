#!/usr/bin/python3

import os

import numpy as np
import pandas as pd


def parse_node_data(name="data_5_nodes.csv"):
    """Function that parses node data collected using Cooja Simulator

    Args:
        name (str, optional): [description]. Defaults to "data_5_nodes.csv".
        Filename (if in same dir) or full path of csv file that cotains the data
    """

    node_count = ''.join(filter(str.isdigit, name))

    df = pd.read_csv(name)

    df_30 = pd.concat([df[df['node_id'] == i][:30]
                       for i in range(2, int(node_count) + 2)], ignore_index=True)

    path_f = None
    if os.name == 'nt':
        path_f = name.rfind('\\')
    elif os.name == 'posix':
        path_f = name.rfind('/')
    else:
        pass
    save_path = f'data_{node_count}_30r.csv'

    if path_f:
        save_path = name[:path_f + 1] + save_path

    df_30.to_csv(save_path, index=False)
    df_30_stats = df_30.describe()
    df_30_stats.drop('node_id', axis=1, inplace=True)
    print(df_30_stats)


if __name__ == '__main__':
    parse_node_data(input("Enter the path to the csv file:\n"))
