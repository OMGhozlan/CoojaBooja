import pandas as pd

columns = ['Session Key', 'Protocol', 'Src IP', 'Src Port', 
'Dst IP', 'Dst Port', 'Init Ts', 'Last Ts', 'Ts', 'Duration', 'Mac Addr', 'Type']

for i in range(1, 4):
    df = pd.read_csv(f'a2_{i}_mod.csv')
    df[columns].to_csv(f'a2_{i}_modf.csv')