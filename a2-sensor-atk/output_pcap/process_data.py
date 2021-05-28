import pandas as pd

columns = ['Protocol', 'Src IP', 'Src Port', 
'Dst IP', 'Dst Port', 'Init Ts', 'Last Ts', 'Ts', 'Duration', 'Mac Addr', 'Label']

df = []
for i in range(1, 4):
    df.append(pd.read_csv(f'a2_{i}_labeled.csv'))

df = pd.concat(df)

df[columns].to_csv(f'data.csv', index=False)