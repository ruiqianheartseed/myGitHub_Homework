import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('./datasets/iris.data', header=None) # 加载Iris数据集作为DataFrame对象
#X = df.iloc[:, [0, 1]].values # 取出2个特征，并把它们用Numpy数组表示
#X = df.iloc[:, [0, 2]].values
#X = df.iloc[:, [0, 3]].values
#X = df.iloc[:, [1, 2]].values
#X = df.iloc[:, [1, 3]].values
X = df.iloc[:, [2, 3]].values

plt.scatter(X[:50, 0], X[:50, 1],color='red', marker='o', label='setosa') # 前50个样本的散点图
plt.scatter(X[50:100, 0], X[50:100, 1],color='blue', marker='x', label='versicolor') # 中间50个样本的散点图
plt.scatter(X[100:, 0], X[100:, 1],color='green', marker='+', label='Virginica') # 后50个样本的散点图
plt.xlabel('petal length') # sepal length; sepal width; petal length; petal width;
plt.ylabel('petal width')
plt.legend(loc=2) # 把说明放在左上角，具体请参考官方文档

#plt.savefig("sepalLength0-sepalWidth1.png")
#plt.savefig("sepalLength0-petalLength2.png")
#plt.savefig("sepalLength0-petalWidth3.png")
#plt.savefig("sepalWidth1-petalLength2.png")
#plt.savefig("sepalWidth1-petalWidth3.png")
plt.savefig("petalLength2-petalWidth3.png")

plt.show()