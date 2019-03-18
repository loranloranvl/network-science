import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

dfs = [pd.read_csv(f'dc_equal_m{m}.csv') for m in range(1, 9, 2)]

for df in dfs:
    plt.scatter(df['degree'], np.log10(df['count'] / df['count'].sum()))

plt.show()