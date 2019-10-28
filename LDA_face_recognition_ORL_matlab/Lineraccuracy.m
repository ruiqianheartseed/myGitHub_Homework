function accuracy=Lineraccuracy(testsample,num1,trainsample,num2)
%线性判别准则计算准确率的函数
%输入testsample表示经过投影后的测试样本,num1表示每一类测试样本的个数,
%trainsample代表经过投影后的训练样本,num2代表每一类训练样本的个数
%输出为正确率
accu=0;
testsampnum=size(testsample,1);%测试个数
trainsampnum=size(trainsample,1);%训练个数
for i=1:testsampnum
    for j=1:trainsampnum
        %求每一个测试样本到训练样本之间在投影特征空间中的距离
        juli(j)=norm(testsample(i,:)-trainsample(j,:));
    end
    [temp index]=sort(juli);%到各训练集距离升序排序
    if ceil(i/num1)==ceil(index(1)/num2)%和距离最近的那个样本是否真是同一类
        accu=accu+1;
    end
end
accuracy=accu/testsampnum;
