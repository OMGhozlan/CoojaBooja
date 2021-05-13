#!/usr/bin/python3

import os

import numpy as np
import pandas as pd


def parse_node_data(name='', rounds=30):
    """Function that parses node data collected using Cooja Simulator

    Args:
        name (str, optional): [description]. Defaults to "data_5_nodes.csv".
        Filename (if in same dir) or full path of csv file that cotains the data
    """

    node_count = ''.join(filter(str.isdigit, name))

    df = pd.read_csv(name)
    df = df.sample(frac=1).reset_index(drop=True)
    df_rounds = pd.concat([df[df['node_id'] == i][:rounds]
                           for i in range(2, int(node_count)+2)], ignore_index=True)

    path_f = None
    if os.name == 'nt':
        path_f = name.rfind('\\')
    elif os.name == 'posix':
        path_f = name.rfind('/')
    else:
        pass
    save_path = f'data_{node_count}_{rounds}r.csv'

    if path_f:
        save_path = name[:path_f + 1] + save_path

    df_rounds.to_csv(save_path, index=False)
    df_rounds_stats = df_rounds.describe()
    df_rounds_stats.drop('node_id', axis=1, inplace=True)
    print(df_rounds_stats)

def _input_handler(input_, type_):
    """Helper function that handles user input

    Args:
        input_ (str): Input provided by the user via input()
        type_ (str): parameter being used
    Raises:
        ValueError: Error in case of unknown type_(parameter) passed

    Returns:
        str : string value of selected parameter
    """
    if type_ == "rounds":
        try:
            rounds = int(input_)
        except ValueError:
            print("30 rounds as default")
            rounds = "30"
        return rounds           
    elif type_ == "file":
        if not input_:
            print("Using data_5_nodes as default")
            return "data_5_nodes.csv"
    else:
        raise ValueError("Unknown input type")


if __name__ == '__main__':
    data_file = _input_handler(input("Enter the path to the csv file: ")
    , "file")
    rounds = int(_input_handler(input("Enter number of rounds: "), "rounds"))
    parse_node_data(name=data_file, rounds=rounds)
    
