import numpy as np
import pandas as pd
from scipy import stats
import matplotlib.pyplot as plt

line_colors = ['#1f77b4', '#ff7f0e', '#2ca02c']
points_colors = [c + 'dd' for c in line_colors]
shapes = ['*', '+', '1']

dfs = [pd.read_csv(f'data/dc_t{t}w.csv') for t in [10, 15, 20]]

for i, df in enumerate(dfs):
    plt.subplot(221 + i)

    # draw points
    k = np.log10(df['degree'])
    pk = np.log10(df['count'] / df['count'].sum())

    # linear regression
    k_regressed = []
    pk_regressed = []
    for x, y in zip(k, pk):
        if x < 2:
            k_regressed.append(x)
            pk_regressed.append(y)
    slop, intercept = stats.linregress(k_regressed, pk_regressed)[:2]
    fx = lambda x: slop * x + intercept
    gy = lambda y: (y - intercept) / slop

    # draw
    plt.plot([0.5, gy(-6)], [fx(0.5), -6], '--', c=line_colors[i])
    plt.scatter(k, pk, label=f't = {10 + 5 * i}0,000', marker=shapes[i], color=points_colors[i])

    plt.legend()
    plt.xlabel('k')
    plt.ylabel('P(k)')
    plt.xticks(np.arange(1, 7), [f'$10^{exp}$' for exp in range(1, 7)]);
    plt.yticks(np.arange(-6, 1), [f'$10^{{{exp}}}$' for exp in range(-6, 0)] + ['1']);

    # add together
    plt.subplot(224)
    plt.plot([0.5, gy(-6)], [fx(0.5), -6], '--', c=line_colors[i])
    plt.scatter(k, pk, label=f't = {10 + 5 * i}0,000', marker=shapes[i], color=points_colors[i])

plt.legend()
plt.xlabel('k')
plt.ylabel('P(k)')
plt.xticks(np.arange(1, 7), [f'$10^{exp}$' for exp in range(1, 7)]);
plt.yticks(np.arange(-6, 1), [f'$10^{{{exp}}}$' for exp in range(-6, 0)] + ['1']);

plt.show()