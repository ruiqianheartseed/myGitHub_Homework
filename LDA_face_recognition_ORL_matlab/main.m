clear all
clc
close all
start=clock;
sample_class=1:40;%样本类别，共40人
sample_classnum=size(sample_class,2);%样本类别数，实际为40
address=[pwd '\ORL\s'];%标记当前文件夹下的ORL文件夹内s开头的文件
fprintf('Start....................\n\n')
for train_samplesize=3:8     %取前3~8个样本做训练集
    train=1:train_samplesize;%每类训练样本编号存入数组
    test=(train_samplesize+1):10;%每类测试样本编号存入数组
    train_num=size(train,2);%每类训练样本数，3~8
    test_num=size(test,2);%每类测试样本数，7~2
    %读取训练样本，所有训练样本变形为行向量，组成的train_num*40行矩阵
    trainsamples=Readsample(address,sample_class,train);
    %先使用PCA进行降维，将训练集矩阵的列数减少
    [newtrainsample,base,retrain]=PCA(trainsamples,0.9);
    %输出一下降维前后的图像对比
    %Showface(trainsamples)
    %Showface(retrain)
    %计算Sw,Sb
    [sw,sb]=SwandSb(newtrainsample,sample_classnum,train_num);
    %读取测试样本
    testsample=Readsample(address,sample_class,test);
    best_acc=0;%最优识别率
    %通过改变投影矩阵中k的取值，在Fisher准则下采用遍历法寻找最佳投影维数
    acc=zeros(1,39);
    dim=1:1:39;
    for temp_dimension=1:1:39%实际上最多39个特征
       %求当前所选维度下的最优投影向量
        [vsort1,d1]=FindWk(sw,sb,temp_dimension);
        %训练样本和测试样本分别投影
        tstsample=testsample*base*vsort1;
        trainsample=newtrainsample*vsort1;
        %线性判别准则计算识别率
        accuracy=Lineraccuracy(tstsample,test_num,trainsample,train_num);
        acc(temp_dimension)=accuracy;
        if accuracy>best_acc
            best_dimension=temp_dimension;%保存最佳投影维数
            best_acc=accuracy;
        end
    end
    %绘制准确率变化图像
    %figure(13);
    %plot(dim,acc,'k','LineWidth',1);xlabel('投影维度数量');ylabel('测试准确率');hold on  
    %---------------------------------输出显示----------------------------------
    fprintf('每类训练样本数为：%d\n',train_samplesize);
    fprintf('PCA降维后占有90%%能量的特征维度数为：%d\n',length(sw));
    fprintf('最佳投影向量W维度数k为：%d\n',best_dimension);
    fprintf('FisherFace的识别率为：%.2f%%\n',best_acc*100);
    fprintf('程序运行时间为：%3.2fs\n\n',etime(clock,start));
    %--------------------------------保存数据-----------------------------------
    train_n(train_samplesize-2)=train_samplesize;%每类训练样本数
    pca_n(train_samplesize-2)=length(sw);%PCA降维后占有90%能量的特征维度数
    bacc_n(train_samplesize-2)=best_acc*100;%最高准确率
    bweidu_n(train_samplesize-2)=best_dimension;%最佳维度
end
  %figure(11);
  %plot(train_n,bacc_n,'b','LineWidth',1);xlabel('每类训练样本数');ylabel('最高准确率');hold on
  %figure(12);
  %plot(train_n,bweidu_n,'r','LineWidth',1);xlabel('每类训练样本数');ylabel('投影维度数');hold on
  %plot(train_n,pca_n,'b','LineWidth',1);
 fprintf('End....................\n\n');
