import numpy as np
import matplotlib.pyplot as plt
from iris_utils import load_dataset
'''
此程序用于鸢尾花数据集的分类，预测使用逻辑回归法，参数更新使用梯度下降法。
将结果为0类的和1类的数据各取40个用于训练，将剩余的各10个0类和1类数据用于测试。
'''
# 第一步，载入训练集，训练集标签，测试集，测试集标签
train_set_x_orig, train_set_y, test_set_x_orig, test_set_y = load_dataset()

# 第二步，设定需要的函数
def sigmoid(z):
    """
    sigmoid函数
    :param z: 输入任意一个z
    :return s: 输出sigmoid预测值
    """
    s = 1 / (1 + np.exp(-z))
    return s

def propagate(w, b, X, Y):
    """
    前向传播参数
    :param w: 权重矩阵，此时大小为1*4
    :param b: 偏移量，实数
    :param X: 训练集
    :param Y: 训练集标签
    :return grads cost: 梯度 误差值
    """
    m = X.shape[0]
    A = np.zeros((m,1))
    #正向传播
    for i in range(0, m):
        A[i,0] = sigmoid(np.dot(w,X[i,:].T) + b)
    cost = (- 1 / m) * np.sum(Y * np.log(A) + (1 - Y) * (np.log(1 - A)))
    #反向传播
    dw = (1 / m) * np.dot((A - Y).T,X )
    db = (1 / m) * np.sum(A - Y)
    #使用断言确保我的数据是正确的
    assert(dw.shape == w.shape)
    assert(db.dtype == float)
    cost = np.squeeze(cost)
    assert(cost.shape == ())
    #创建一个字典，把dw和db保存起来。
    grads = {
                "dw": dw,
                "db": db
             }
    return (grads , cost)

def optimize(w , b , X , Y , num_iterations , learning_rate , print_cost = False):
    """
    使用已经计算好的更新值更新参数
    :param w: 权重矩阵
    :param b: 偏移量
    :param X: 训练集
    :param Y: 训练集标签
    :param num_iterations: 迭代次数
    :param learning_rate: 学习率
    :param print_cost: 是否打印此时的误差值
    :return params grads costs: 返回更新后的参数w,b，梯度，误差值
    """
    costs = []
    for i in range(num_iterations):

        grads, cost = propagate(w, b, X, Y)

        dw = grads["dw"]
        db = grads["db"]

        w = w - learning_rate * dw
        b = b - learning_rate * db

        #记录成本
        if i % 100 == 0:
            costs.append(cost)
        #打印成本数据
        if (print_cost) and (i % 100 == 0):
            print("迭代的次数: %i ， 误差值： %f" % (i,cost))
    params  = {
                "w" : w,
                "b" : b }
    grads = {
            "dw": dw,
            "db": db }
    return (params , grads , costs)

def predict(w , b , X ):
    """
    检测预测模型的准确率
    :param w: 权重矩阵
    :param b: 偏移量
    :param X: 预测集
    :return Y_prediction, A: 返回实际预测值，sigmoid计算后的值
    """
    m  = X.shape[0]
    A = np.zeros((m, 1))
    Y_prediction = np.zeros((m,1))

    #计预测花出现的概率
    for i in range (0,m):
        A[i,0] = sigmoid(np.dot(w , X[i,:].T) + b)

    for i in range(A.shape[0]):
        #将概率a [0，i]转换为实际预测p [0，i]
        Y_prediction[i,0] = 1 if A[i,0] > 0.5 else 0
    #使用断言
    assert(Y_prediction.shape == (m,1))

    return (Y_prediction, A)

# 第三步，将设定好的函数打包，方便调用
def model(X_train , Y_train , X_test , Y_test , num_iterations = 2000 , learning_rate = 0.5 , print_cost = False):
    """
    函数打包实现训练更新测试一体化
    :param X_train: 训练集
    :param Y_train: 训练集标签
    :param X_test: 测试集
    :param Y_test: 测试集标签
    :param num_iterations: 迭代次数
    :param learning_rate: 学习率
    :param print_cost: 是否打印损失值
    :return d: 包含含有各种重要参数的字典
    """
    w = np.array([[0.1,0.1,-0.1,-0.1]])
    b = 0.1
    parameters , grads , costs = optimize(w , b , X_train , Y_train,num_iterations , learning_rate , print_cost)
    #从字典“参数”中检索参数w和b
    w , b = parameters["w"] , parameters["b"]
    #预测测试/训练集的例子
    (Y_prediction_test,A_test) = predict(w , b, X_test)
    (Y_prediction_train,A_train) = predict(w , b, X_train)
    #打印训练后的准确性
    print("训练集准确性："  , format(100 - np.mean(np.abs(Y_prediction_train - Y_train)) * 100) ,"%")
    print("测试集准确性："  , format(100 - np.mean(np.abs(Y_prediction_test - Y_test)) * 100) ,"%")
    d = {
            "costs" : costs,
            "Y_prediction_test" : Y_prediction_test,
            "Y_prediciton_train" : Y_prediction_train,
            "w" : w,
            "b" : b,
            "learning_rate" : learning_rate,
            "num_iterations" : num_iterations }
    print("W =\n" , w)
    print("b =" , b)
    return d

# 第四步，将训练集和测试集输入打包好的函数，设定更新率和更新次数，观察结果
d = model(train_set_x_orig, train_set_y, test_set_x_orig, test_set_y, num_iterations = 15000+1, learning_rate = 0.01, print_cost = True)

# 绘制图
costs = np.squeeze(d['costs'])
plt.plot(costs)
plt.ylabel('cost')
plt.xlabel('iterations (per hundreds)')
plt.title("Learning rate =" + str(d["learning_rate"]))
plt.savefig("LogisticCost01.png")

# 学习率检测
learning_rates = [0.001, 0.01, 0.1, 1]
models = {}
for i in learning_rates:
    print ("learning rate is: " + str(i))
    models[str(i)] = model(train_set_x_orig, train_set_y, test_set_x_orig, test_set_y, num_iterations = 15000+1, learning_rate = i, print_cost = False)
    print ('\n' + "-------------------------------------------------------" + '\n')
for i in learning_rates:
    plt.plot(np.squeeze(models[str(i)]["costs"]), label= str(models[str(i)]["learning_rate"]))
plt.ylabel('cost')
plt.xlabel('iterations (per hundreds)')
legend = plt.legend(loc='upper center', shadow=True)
frame = legend.get_frame()
frame.set_facecolor('0.90')
plt.savefig("LogisticCostAll.png")
plt.show()


