#!/usr/bin/env python

# --------------------
# **ライブラリ管理**
# --------------------
import numpy as np
import matplotlib.pyplot as pylab

def Sigmoid_F(x):
    return 1 / (1 + np.exp(-x))

def uni_hosu(x):
    return 1 - x

class LogisticReg:
    def __init__(self, datasets):
        self._dim = 0
        # 入力データセットの分割数
        self._set_num = len(datasets)
        self._coef = np.zeros( (self._dim, 1) )
        self._bias = 0

    def test(self):
        print(self._dim)

    def fitting(self, train_data):
        # Step 1    Init
        # 標準正規分布による乱数
        self._dim = len(train_data[0][0])
        self._coef = np.random.randn( self._dim, 1 )
        self._bias = np.random.randn(  )
        alpha = 1


        train_feature_vec = np.array( [x[0] for x in train_data] )
        cor_label_vec = np.array( [x[1] for x in train_data] )
        print(train_feature_vec)

        for x in range(10000):
            fit_target_index = np.random.randint(0, len(train_data) - 1)
            predict_per_class = Sigmoid_F( ( self._coef.T @ np.reshape(train_feature_vec[fit_target_index, :].T, (2,1)) ) + self._bias )

            self._coef = self._coef - \
                         alpha * ( predict_per_class - cor_label_vec[fit_target_index] ) * \
                         np.reshape(train_feature_vec[fit_target_index, :].T, (2,1))

            self._bias = self._bias - alpha * ( predict_per_class - cor_label_vec[fit_target_index] )

            e = np.sum(-1 * ( cor_label_vec * np.log(Sigmoid_F( train_feature_vec @ self._coef + self._bias )) + \
                uni_hosu(cor_label_vec) * uni_hosu(np.log(Sigmoid_F( train_feature_vec @ self._coef + self._bias )))) )
            print(e)
            print(self._coef, self._bias)
            alpha *= 0.999

        pylab.figure(facecolor="w")

        pylab.scatter(train_feature_vec[:, 0], train_feature_vec[:, 1], color='r', marker='x', label="$ClassA$")
        pylab.xlabel('$x$',size=20)
        pylab.ylabel('$y$',size=20)
        pylab.axis([-10.0,10.0,-10.0,10.0],size=20)
        pylab.grid(True)
        pylab.legend()

        pylab.show()
