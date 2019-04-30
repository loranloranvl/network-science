import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import h5py

del_line = '\r\x1b[K'

def pr_cleaned(df):
    '''
    data clean process
    remove dangling links

    Parameters
    ----------
    df: pandas.dataframe
        raw dataframe

    Returns
    -------
    cdf: pandas.dataframe
        cleaned dataframe

    danglings: list
        row indices of the removed
    '''

    cdf = df.copy()
    lcdf = len(cdf)

    danglings = []

    print('cleaning data frame')
    for index, row in cdf.iterrows():
        if index in danglings:
            cdf = cdf.drop(index)
        elif not (cdf['from'] == row['to']).any():
            danglings.append(index)
            cdf = cdf.drop(index)

        if not index % 77:
            print(f'{del_line}{index / lcdf * 100:.1f}%', end='')
    print(del_line + 'data cleaned')

    return cdf, np.array(danglings)


if __name__ == '__main__':
    hdf_path = 'data/datasets.h5'
    hdf = h5py.File(hdf_path)

    if 'cdf' not in hdf:
        df = pd.read_csv('data/web-Google.txt', sep='\t', 
            skiprows=4, header=None, names=['from', 'to'])[:1000]

        cdf, danglings = pr_cleaned(df)
        cdf.to_hdf(hdf_path, 'cdf')
        np.save('data/danglings.npy', danglings)

    else:
        cdf = pd.read_hdf(hdf_path, 'cdf')
        danglings = np.load('data/danglings.npy')

    print(cdf)
