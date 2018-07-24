import numpy as np
import matplotlib.pyplot as pylab
import random
import csv

# 分割用関数 リストと分割数を入力
def split_array(ar, n_group):
    for i_chunk in range(n_group):
        yield ar[i_chunk * len(ar) // n_group:(i_chunk + 1) * len(ar) // n_group]

# クラス毎のデータ数
data_num = 500
# 分割数
split_num = 5

if __name__ == '__main__':

    # 平均、共分散行列
    mu_A = [4, -4]
    mu_B = [-3, 3]

    cov_A =  [  [2, -1],
                [-1, 3]  ]
    cov_B =  [  [3, 2],
                [2, 4]  ]

    # データ生成
    A = np.array(np.random.multivariate_normal(mu_A, cov_A, data_num))
    A_out = np.hstack((A, np.ones((500, 1), dtype="int8")))
    B = np.array(np.random.multivariate_normal(mu_B, cov_B, data_num))
    B_out = np.hstack((A, np.zeros((500, 1), dtype="int8")))

    #  データシャッフル、クラスラベルの調整、５分割
    output_list = random.sample( np.vstack( (A_out, B_out) ).tolist() , len(np.vstack( (A_out, B_out) ).tolist()) )
    output_list = [ [x[0], x[1], int(x[2])] for x in output_list ] #0, 1をintにしてるだけ
    output_list = [ list(x) for x in split_array(output_list, split_num)  ]
    for x in range(split_num):
        file_name = "easy2class_" + str(x) + ".csv"
        with open(file_name, 'w') as f:
            writer = csv.writer(f, lineterminator='\n')
            writer.writerows(output_list[x])

    # 以下描画用コード
    pylab.figure(facecolor="w")

    pylab.scatter(A[:, 0], A[:, 1], color='r', marker='x', label="$ClassA$")
    pylab.scatter(B[:, 0], B[:, 1], color='b', marker='x', label="$ClassB$")
    pylab.xlabel('$x$',size=20)
    pylab.ylabel('$y$',size=20)
    pylab.axis([-10.0,10.0,-10.0,10.0],size=20)
    pylab.grid(True)
    pylab.legend()

    pylab.show()
