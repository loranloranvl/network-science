import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import math

colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', 
    '#9467bd', '#8c564b', '#e377c2', '#7f7f7f', 
    '#bcbd22', '#17becf']

df = pd.read_csv('data/graph-properties.csv')
p = np.log10(df['p'][:-1])
Cp, C0 = df['Cp'][:-1], df['Cp'][len(df)-1]
Lp, L0 = df['Lp'][:-1], df['Lp'][len(df)-1]
Tp, T0 = df['Tp'][:-1], df['Tp'][len(df)-1]
rhalf = df['rhalf'][:-1]


def draw_prop():
    plt.figure()
    plt.scatter(p, Lp/L0, marker='d', facecolors='none', 
        edgecolors=colors[1], label=r'$L(p)\ /\ L(0)$')
    plt.scatter(p, Cp/C0, marker='^', facecolors='none', 
        edgecolors=colors[0], label=r'$C(p)\ /\ C(0)$')


def draw_rhalf():
    plt.figure()
    plt.scatter(p, rhalf, marker='p', facecolors='none',
        edgecolors=colors[2], label='$r_{half}$')
    plt.ylabel('$r_{half}$')


def draw_tp():
    plt.figure()
    plt.scatter(p, Lp/L0, marker='d', facecolors='none', 
        edgecolors=colors[1], label=r'$L(p)\ /\ L(0)$')
    plt.scatter(p, Tp/T0, marker='8', facecolors='none', 
        edgecolors=colors[3], label=r'$T(p)\ /\ T(0)$')


def x_axis():
    plt.xlabel('p')
    x_range = np.arange(-4, 1)
    plt.xticks(x_range, [str(math.pow(10, i)) for i in x_range])
    plt.legend()
    

draw_prop()
x_axis()
plt.savefig('prop.eps', format='eps', dpi=1000)
draw_rhalf()
x_axis()
plt.savefig('rhalf.eps', format='eps', dpi=1000)
draw_tp()
x_axis()
plt.savefig('tp.eps', format='eps', dpi=1000)


# plt.show()