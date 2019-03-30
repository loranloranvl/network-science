import numpy as np
import pandas as pd
from scipy import stats
import matplotlib.pyplot as plt

line_colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
points_colors = [c + 'dd' for c in line_colors]
shapes = ['*', '+', '1', '.']

dfs = [pd.read_csv(f'data/dc_growth_m{m}.csv') for m in range(1, 9, 2)]

for i, df in enumerate(dfs):
    # draw points
    k = df['degree']
    pk = np.log10(df['count'] / df['count'].sum())

    # linear regression
    slop, intercept = stats.linregress(k, pk)[:2]
    fx = lambda x: slop * x + intercept
    gy = lambda y: (y - intercept) / slop

    # draw
    plt.plot([gy(-1), gy(-4)], [-1, -4], '--', c=line_colors[i])
    plt.scatter(k, pk, label=f't = {10 + 5 * i}0,000', marker=shapes[i], color=points_colors[i])

plt.legend()
plt.xlabel('k')
plt.ylabel('P(k)')
# plt.xticks(np.arange(1, 7), [f'$10^{exp}$' for exp in range(1, 7)]);
plt.yticks(np.arange(-4, 1), [f'$10^{{{exp}}}$' for exp in range(-4, 0)] + ['1']);

plt.show()