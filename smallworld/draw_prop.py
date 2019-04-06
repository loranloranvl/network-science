import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import math

df = pd.read_csv('data/graph-properties.csv')
p = np.log10(df['p'][:-1])
Cp, C0 = df['Cp'][:-1], df['Cp'][len(df)-1]
Lp, L0 = df['Lp'][:-1], df['Lp'][len(df)-1]

plt.scatter(p, Cp/C0, marker='^', facecolors='none', 
    edgecolors='#1f77b4', label=r'$C(p)\ /\ C(0)$')
plt.scatter(p, Lp/L0, marker='d', facecolors='none', 
    edgecolors='#ff7f0e', label=r'$L(p)\ /\ L(0)$')

plt.xlabel('p')
x_range = np.arange(-4, 1)
plt.xticks(x_range, [str(math.pow(10, i)) for i in x_range])

plt.legend()

plt.show()