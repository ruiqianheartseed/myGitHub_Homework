clear all
clc
close all
start=clock;
sample_class=1:40;%������𣬹�40��
sample_classnum=size(sample_class,2);%�����������ʵ��Ϊ40
address=[pwd '\ORL\s'];%��ǵ�ǰ�ļ����µ�ORL�ļ�����s��ͷ���ļ�
fprintf('Start....................\n\n')
for train_samplesize=3:8     %ȡǰ3~8��������ѵ����
    train=1:train_samplesize;%ÿ��ѵ��������Ŵ�������
    test=(train_samplesize+1):10;%ÿ�����������Ŵ�������
    train_num=size(train,2);%ÿ��ѵ����������3~8
    test_num=size(test,2);%ÿ�������������7~2
    %��ȡѵ������������ѵ����������Ϊ����������ɵ�train_num*40�о���
    trainsamples=Readsample(address,sample_class,train);
    %��ʹ��PCA���н�ά����ѵ�����������������
    [newtrainsample,base,retrain]=PCA(trainsamples,0.9);
    %���һ�½�άǰ���ͼ��Ա�
    %Showface(trainsamples)
    %Showface(retrain)
    %����Sw,Sb
    [sw,sb]=SwandSb(newtrainsample,sample_classnum,train_num);
    %��ȡ��������
    testsample=Readsample(address,sample_class,test);
    best_acc=0;%����ʶ����
    %ͨ���ı�ͶӰ������k��ȡֵ����Fisher׼���²��ñ�����Ѱ�����ͶӰά��
    acc=zeros(1,39);
    dim=1:1:39;
    for temp_dimension=1:1:39%ʵ�������39������
       %��ǰ��ѡά���µ�����ͶӰ����
        [vsort1,d1]=FindWk(sw,sb,temp_dimension);
        %ѵ�������Ͳ��������ֱ�ͶӰ
        tstsample=testsample*base*vsort1;
        trainsample=newtrainsample*vsort1;
        %�����б�׼�����ʶ����
        accuracy=Lineraccuracy(tstsample,test_num,trainsample,train_num);
        acc(temp_dimension)=accuracy;
        if accuracy>best_acc
            best_dimension=temp_dimension;%�������ͶӰά��
            best_acc=accuracy;
        end
    end
    %����׼ȷ�ʱ仯ͼ��
    %figure(13);
    %plot(dim,acc,'k','LineWidth',1);xlabel('ͶӰά������');ylabel('����׼ȷ��');hold on  
    %---------------------------------�����ʾ----------------------------------
    fprintf('ÿ��ѵ��������Ϊ��%d\n',train_samplesize);
    fprintf('PCA��ά��ռ��90%%����������ά����Ϊ��%d\n',length(sw));
    fprintf('���ͶӰ����Wά����kΪ��%d\n',best_dimension);
    fprintf('FisherFace��ʶ����Ϊ��%.2f%%\n',best_acc*100);
    fprintf('��������ʱ��Ϊ��%3.2fs\n\n',etime(clock,start));
    %--------------------------------��������-----------------------------------
    train_n(train_samplesize-2)=train_samplesize;%ÿ��ѵ��������
    pca_n(train_samplesize-2)=length(sw);%PCA��ά��ռ��90%����������ά����
    bacc_n(train_samplesize-2)=best_acc*100;%���׼ȷ��
    bweidu_n(train_samplesize-2)=best_dimension;%���ά��
end
  %figure(11);
  %plot(train_n,bacc_n,'b','LineWidth',1);xlabel('ÿ��ѵ��������');ylabel('���׼ȷ��');hold on
  %figure(12);
  %plot(train_n,bweidu_n,'r','LineWidth',1);xlabel('ÿ��ѵ��������');ylabel('ͶӰά����');hold on
  %plot(train_n,pca_n,'b','LineWidth',1);
 fprintf('End....................\n\n');
