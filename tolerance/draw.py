import numpy as np
import matplotlib.pyplot as plt

colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', 
    '#9467bd', '#8c564b', '#e377c2', '#7f7f7f', 
    '#bcbd22', '#17becf']

x = np.linspace(0, 0.05, 20)
sigma = 0.014

exp_failure = 26 * x + 6.9 + np.random.normal(0, sigma, 20)
plt.scatter(x, exp_failure, facecolor='none', edgecolor=colors[0], marker='^', label='E_failure')

exp_attack = 26 * (x - 0.001) + 6.9 + np.random.normal(0, sigma, 20)
plt.scatter(x - 0.001, exp_attack, facecolor='none', edgecolor=colors[1], marker='D', label='E_attack')

sf_failure = 5.4 + np.random.normal(0, sigma, 20)
plt.scatter(x, sf_failure, facecolor='none', edgecolor=colors[0], marker='s', label='SF_failure')

sf_attack = 94 * x + 5.8
plt.scatter(x - 0.001, sf_attack, facecolor='none', edgecolor=colors[1], marker='o', label='SF_attack')

plt.ylabel(r'$d$ (diameter)')
plt.xlabel(r'$f$ (fraction of label removed)')

plt.legend()
plt.savefig('tolerance.eps', format='eps')