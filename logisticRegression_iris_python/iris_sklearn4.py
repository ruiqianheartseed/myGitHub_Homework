from sklearn import datasets
import numpy as np
from sklearn.model_selection import  train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import ShuffleSplit
'''此函数用于鸢尾花数据集的one_vs_all的三元分类，
使用4个特征数据作为输入，观察系统准确度相比两个特征值是否有变换'''
# 载入数据集，提取80%训练集与20%验证集
iris = datasets.load_iris()
X = iris.data
y = iris.target
X_train, X_test, y_train, y_test = train_test_split(X, y,train_size=0.8, test_size=0.2, random_state=0)

# 全体数据标准化，方便系统判别
sc = StandardScaler()
sc.fit(X_train)
X_train_std = sc.transform(X_train)
X_test_std = sc.transform(X_test)
print(X_test_std,'\n',y_test )

# 开始逻辑回归，因为归一化后系数矩阵值都不大，正则化系数仅取1*10^-3
lr = LogisticRegression(C=1000.0, random_state=0)
lr.fit(X_train_std, y_train)
print(lr.predict_proba([X_test_std[0,:]])) # 查看第一个测试样本属于各个类别的概率

# 使用5轮交叉验证的平均值来判断系统准确度，输出最终的系数矩阵与平方累计误差
cv_split = ShuffleSplit(n_splits=5, train_size=0.8, test_size=0.2)
score_ndarray = cross_val_score(lr, X_train_std, y_train, cv=cv_split)
print(score_ndarray.mean())
print("Coefficients:\n%s\nintercept:\n %s\n"%(lr.coef_,lr.intercept_))
print("Residual sum of squares: %.2f"% np.mean((lr.predict(X_test_std) - y_test) ** 2))
