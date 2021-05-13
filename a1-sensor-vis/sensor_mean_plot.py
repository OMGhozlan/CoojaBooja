import itertools

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# groups_ = ['5n, 2r', '30n, 2r', '5n, 30r', '30n, 30r', '5n, 100r', '30n, 100r']
groups_ = ['5n, 2r', '5n, 30r', '5n, 100r', '30n, 2r', '30n, 30r', '30n, 100r']

node_count = [5, 30]
rounds = [2, 30, 100]

columns = ['temperature', 'humidity', 'pressure']
values_o = [25.0, 40.0, 101000.0]

# count, mean, std, min, 25, 50, 75, max

stats_dfs = []
temp_means, humid_means, pressure_means = [], [], []
mean_arr = [temp_means, humid_means, pressure_means]
for n, r in itertools.product(node_count, rounds):
    df = pd.read_csv(f'./data/data_stats_{n}_{r}r.csv')
    print(f'For {n} nodes and {r} rounds')
    for col, val, mean_ in zip(columns, values_o, mean_arr,):
        column = df[col]
        for er_lbl, val_idx in zip(['Mean', 'Min', 'Max'], [1, 3, 7]):            
            err_val = column[val_idx] - val
            print(f'[*]{er_lbl} error for {col} is {err_val}')
        mean_.append(column[1] - val)
    stats_dfs.append(df)


label_width = 0.15  
x = np.arange(len(groups_))
y = np.arange(0, 1.5, 0.1)


fig, ax = plt.subplots()
rects_temp = ax.bar(x - label_width, temp_means, label_width, label='Temperature')
rects_humid = ax.bar(x, humid_means, label_width, label='Humidity')
rects_press = ax.bar(x + label_width, pressure_means, label_width, label='Pressure')

"""for i in x:
    ax.text(x=i - label_width, y=0.5, s=str(temp_means[i]), fontsize=5, rotation=90.0)
    ax.text(x=i, y=0.5, s=str(humid_means[i]), fontsize=5, rotation=90.0)
    ax.text(x=i + label_width, y=0.5, s=str(pressure_means[i]), fontsize=5, rotation=90.0)
    """

for rect in ax.patches:
    bar_height = rect.get_height()
    ax.annotate(f'{bar_height:.2f}',
                xy=(rect.get_x() + rect.get_width() / 3, bar_height),
                xytext=(0, 3), fontsize=10,
                textcoords="offset points", rotation=80.0,
                ha='center', va='bottom')

ax.set_ylabel('Mean reported sensor value')
ax.set_title('Mean errors (with respect to count and rounds)')
ax.set_xticks(x)
ax.set_yticks(y)
ax.set_xticklabels(groups_)
ax.get_yaxis().set_visible(False)
ax.legend()

fig.tight_layout()

plt.show()
