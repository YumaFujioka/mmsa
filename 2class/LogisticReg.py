#!/usr/bin/env python

# --------------------
# **ライブラリ管理**
# --------------------
import numpy as np
import matplotlib.pyplot as pylab
import copy

def Sigmoid_F(x):
    return 1 / (1 + np.exp(-x))

def uni_hosu(x):
    return 1 - x

def MakeListCSVasXY(List):
    return [ [ np.array(  x[:-1] ), x[-1] ] for x in List]

class LogisticReg:
    def __init__(self):
        self._dim = 0
        self._set_num = 0
        self._coef = np.zeros( (self._dim, 1) )
        self._bias = 0

    def test(self):
        print(self._dim)

    def fitting(self, train_data, alpha=1):
        # Step 1    Init
        # 標準正規分布による乱数
        self._dim = len(train_data[0][0])
        self._coef = np.random.randn( self._dim, 1 )
        self._bias = np.random.randn()

        train_feature_vec = np.array( [x[0] for x in train_data] )
        cor_label_vec = np.array( [x[1] for x in train_data] )

        for x in range(5000):
            fit_target_index = np.random.randint(0, len(train_data) - 1)
            #  なおすこと！！！
            predict_per_class = Sigmoid_F( ( self._coef.T @ np.reshape(train_feature_vec[fit_target_index, :].T, (2,1)) ) + self._bias )

            self._coef = self._coef - \
                         alpha * ( predict_per_class - cor_label_vec[fit_target_index] ) * \
                         np.reshape(train_feature_vec[fit_target_index, :].T, (2,1))
            self._bias = self._bias - alpha * ( predict_per_class - cor_label_vec[fit_target_index] )

            # e = np.sum(-1 * ( cor_label_vec * np.log(Sigmoid_F( train_feature_vec @ self._coef + self._bias )) + \
            #     uni_hosu(cor_label_vec) * uni_hosu(np.log(Sigmoid_F( train_feature_vec @ self._coef + self._bias )))) )
            alpha *= 0.999

        # count = 0
        # for x in range(200):
        #     if Sigmoid_F( ( self._coef.T @ np.reshape(train_feature_vec[x, :].T, (2,1)) ) + self._bias ) >= 0.5 and int(cor_label_vec[x]) == 1 :
        #         count += 1
        #     if Sigmoid_F( ( self._coef.T @ np.reshape(train_feature_vec[x, :].T, (2,1)) ) + self._bias ) <= 0.5 and int(cor_label_vec[x]) == 0 :
        #         count += 1
        # print(count)

        # --------------------
        # pylab.figure(facecolor="w")
        #
        # x = np.linspace(-10,10,100)
        # y = np.reshape(-(self._coef[0] * x + self._bias)/self._coef[1], 100)
        # print(x.shape, y.shape)
        # pylab.plot(x,y,"g-")
        #
        # pylab.scatter(train_feature_vec[:, 0], train_feature_vec[:, 1], color='r', marker='x', label="$ClassA$")
        # pylab.xlabel('$x$',size=20)
        # pylab.ylabel('$y$',size=20)
        # pylab.axis([-10.0,10.0,-10.0,10.0],size=20)
        # pylab.grid(True)
        # pylab.legend()
        #
        # pylab.show()

    def CrossVari(self, datasets):
        self._set_num = len(datasets)
        accu_list = []
        Master = datasets
        # print(Master[0])
        for ite in range(self._set_num):
            datasets = copy.deepcopy(Master)
            testset = MakeListCSVasXY(datasets.pop(ite))
            trainset = []
            # print(testset)
            for x in range(len(datasets)):
                trainset.extend(MakeListCSVasXY(datasets[x]))
            self.fitting(trainset)

            count = 0
            test_feature_vec = np.array( [x[0] for x in testset] )
            cor_label_vec = np.array( [x[1] for x in testset] )
            for x in range(len(testset)):
                if Sigmoid_F( ( self._coef.T @ np.reshape(test_feature_vec[x, :].T, (2,1)) ) + self._bias ) >= 0.5 and int(cor_label_vec[x]) == 1 :
                    count += 1
                if Sigmoid_F( ( self._coef.T @ np.reshape(test_feature_vec[x, :].T, (2,1)) ) + self._bias ) <= 0.5 and int(cor_label_vec[x]) == 0 :
                    count += 1
            print("cross ", ite, "accu :\t", count/len(testset))
            accu_list.append(count/len(testset))
            self.plt_data(trainset)
            self.plt_data(testset)
            trainset.extend(testset)
            self.plt_data(trainset, graphsize=5)
        print("\t", self._set_num , "- cross vari : \t", np.mean(accu_list))

    def plt_data(self, data_XY_class, graphsize=5):
        A = []
        B = []

        self._coef = np.reshape(np.array([1.191, -1.339]) , (2,1))
        self._bias = 0

        for pair in data_XY_class:
            if int(pair[1]) == 1:
                A.append(pair)
            else:
                B.append(pair)

        A = np.array( [x[0] for x in A] )
        B = np.array( [x[0] for x in B] )

        pylab.figure(facecolor="w")

        x = np.linspace(-10,10,100)
        y = np.reshape(-(self._coef[0] * x + self._bias)/self._coef[1], 100)
        print(x.shape, y.shape)
        pylab.plot(x,y,"g-")

        pylab.scatter(A[:, 0], A[:, 1], color='r', marker='x', label="$Class1$")
        pylab.scatter(B[:, 0], B[:, 1], color='b', marker='x', label="$Class0$")
        pylab.xlabel('$x$',size=20)
        pylab.ylabel('$y$',size=20)
        pylab.axis([-graphsize/2,graphsize/2,-graphsize/2,graphsize/2],size=graphsize)
        pylab.grid(True)
        pylab.legend()

        pylab.show()
