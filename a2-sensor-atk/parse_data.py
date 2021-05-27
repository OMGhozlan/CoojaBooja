#!/usr/bin/python3

import os

import numpy as np
import pandas as pd


def parse_node_data(name='', rounds=30, to_csv=False):
    """Function that parses node data collected using Cooja Simulator

    Args:
        name (str, optional): Filename (if in same dir) or full path 
        of csv file that cotains the data. Defaults to ''            
        rounds (int, optional): number of rounds performed by sensor
         Defaults to 30.
        to_csv (bool, optional): Save results to csv file. Defaults to False.

    Returns:
        pandas.DataFrame: Data that fit the criteria
    """
    path_f = None
    stats = ''
    if os.name == 'nt':
        path_f = name.rfind('\\')
    elif os.name == 'posix':
        path_f = name.rfind('/')
    else:
        pass
    if path_f:
        node_count = ''.join(filter(str.isdigit, name[path_f + 1:]))
    else:
        node_count = ''.join(filter(str.isdigit, name))

    df = pd.read_csv(name)
    df = df.sample(frac=1).reset_index(drop=True)
    df_rounds = pd.concat([df[df['node_id'] == i][:rounds]
                           for i in range(2, int(node_count)+2)], ignore_index=True)

    df_rounds_stats = df_rounds.describe()
    df_rounds_stats.drop('node_id', axis=1, inplace=True)
    if to_csv:
        path_opts = '.' or name[:path_f + 1]
        print(f'{path_opts}/data/data{stats}_{node_count}_{rounds}r.csv')
        df_rounds.to_csv(f'{path_opts}/data/data{stats}_{node_count}_{rounds}r.csv', index=False)
        stats = '_stats'
        df_rounds_stats.to_csv(f'{path_opts}/data/data{stats}_{node_count}_{rounds}r.csv', index=False)
    return df_rounds_stats



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
            name = "./data/data_5_nodes.csv"
        else:
            name = input_
        return name
            
    else:
        raise ValueError("Unknown input type")


if __name__ == '__main__':
    data_file = _input_handler(input("Enter the path to the csv file[/data/data_5_nodes.csv]: ")
    , "file")
    rounds = int(_input_handler(input("Enter number of rounds[30]: "), "rounds"))
    print(parse_node_data(name=data_file, rounds=rounds, to_csv=True))


