#!/usr/bin/env python

# --------------------
# **パッケージ管理**
# --------------------
from os import path
import numpy as np
import csv
import glob
import LogisticReg

# --------------------
# **ファイル管理**
# --------------------
# このプログラムがあるディレクトリ絶対パスを取得
script_dir = path.abspath(path.dirname(__file__))

csv_files1 = glob.glob( path.join(script_dir, 'data_gen_20180724/*.csv') )
csv_files2 = glob.glob( path.join(script_dir, 'hard_data/*.csv') )

# --------------------
# **関数部**
# --------------------
def MakeListCSVasXY(List):
    return [ [ np.array(  x[:-1] ), x[-1] ] for x in List]

# --------------------
# **メイン部**
# --------------------
if __name__ == '__main__':

    # --------------------
    # **ロード部**
    # --------------------
    dataset = []
    for file in csv_files1:
        with open(file) as f:
            reader = csv.reader(f)
            dataset.append( [[float(x[0]), float(x[1]), int(x[2])] for x in reader] )

    dataset_h = []
    for file in csv_files2:
        with open(file) as f:
            reader = csv.reader(f)
            dataset_h.append( [[float(x[0]), float(x[1]), int(float(x[2]))] for x in reader] )

    LogR = LogisticReg.LogisticReg()
    # LogR.CrossVari(dataset_h)
    LogR.CrossVari(dataset_h)
    # LogR.fitting(MakeListCSVasXY(dataset_h[0]))
