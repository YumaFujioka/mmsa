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

csv_files = glob.glob( path.join(script_dir, 'data_gen_20180724/*.csv') )

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
    for file in csv_files:
        with open(file) as f:
            reader = csv.reader(f)
            dataset.append( [[float(x[0]), float(x[1]), int(x[2])] for x in reader] )

    # print( MakeListCSVasXY(dataset[0]) )
    print(dataset[0])
    LogR = LogisticReg.LogisticReg(MakeListCSVasXY(dataset))
    LogR.fitting(MakeListCSVasXY(dataset[0]))
