from sklearn import datasets
import numpy as np
from sklearn.model_selection import  train_test_split
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from matplotlib.colors import ListedColormap
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import ShuffleSplit
'''此函数用于鸢尾花数据集的one_vs_all分类，
尝试仅使用第3，4列两个特征数据作为输入，以便于观察多分类时的决策平面'''
# 载入数据集，提取80%训练集与20%验证集
iris = datasets.load_iris()
X = iris.data[:, [2, 3]]
y = iris.target
X_train, X_test, y_train, y_test = train_test_split(X, y,train_size=0.8, test_size=0.2, random_state=0)

# 全体数据标准化，方便系统判别
sc = StandardScaler()
sc.fit(X_train)
X_train_std = sc.transform(X_train)
X_test_std = sc.transform(X_test)

# 作图函数，绘制决策平面
X_combined_std = np.vstack((X_train_std, X_test_std))
y_combined = np.hstack((y_train, y_test))
def plot_decision_regions(X, y, classifier, test_idx=None, resolution=0.02):
    # setup marker generator and color map
    markers = ('s', 'x', 'o', '^', 'v')
    colors = ('red', 'blue', 'lightgreen', 'gray', 'cyan')
    cmap = ListedColormap(colors[:len(np.unique(y))])
    # plot the decision surface
    x1_min, x1_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    x2_min, x2_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx1, xx2 = np.meshgrid(np.arange(x1_min, x1_max, resolution), np.arange(x2_min, x2_max, resolution))
    Z = classifier.predict(np.array([xx1.ravel(), xx2.ravel()]).T)
    Z = Z.reshape(xx1.shape)
    plt.contourf(xx1, xx2, Z, alpha=0.4, cmap=cmap)
    plt.xlim(xx1.min(), xx1.max())
    plt.ylim(xx2.min(), xx2.max())
    # plot class samples
    for idx, cl in enumerate(np.unique(y)):
        plt.scatter(x=X[y == cl, 0], y=X[y == cl, 1],alpha=0.8, c=cmap(idx),marker=markers[idx], label=cl)
    # highlight test samples
    if test_idx:
        X_test, y_test = X[test_idx, :], y[test_idx]
        plt.scatter(X_test[:, 0], X_test[:, 1], c='', alpha=1.0, linewidth=1, marker='o', s=55, label='test set')

# 开始逻辑回归，因为归一化后系数矩阵值都不大，正则化系数仅取1*10^-3
lr = LogisticRegression(C=1000.0, random_state=0)
lr.fit(X_train_std, y_train)
lr.predict_proba([X_test_std[0,:]]) # 查看第一个测试样本属于各个类别的概率
plot_decision_regions(X_combined_std, y_combined, classifier=lr, test_idx=range(105,150))
plt.xlabel('petal length [standardized]')
plt.ylabel('petal width [standardized]')
plt.legend(loc='upper left')
plt.savefig(("pl2pw3sk2.png"))


# 使用5轮交叉验证的平均值来判断系统准确度，输出最终的系数矩阵与平方累计误差
cv_split = ShuffleSplit(n_splits=5, train_size=0.8, test_size=0.2)
score_ndarray = cross_val_score(lr, X_train_std, y_train, cv=cv_split)
print(score_ndarray.mean())
print("Coefficients:\n%s\nintercept:\n %s\n"%(lr.coef_,lr.intercept_))
print("Residual sum of squares: %.2f"% np.mean((lr.predict(X_test_std) - y_test) ** 2))
