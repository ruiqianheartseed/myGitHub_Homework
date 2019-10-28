function [newsample,basevector,returnsample]=PCA(patterns,num)
%主分量分析程序，patterns表示输入的原特征矩阵，num为控制变量，
%当num大于1的时候表示要求的特征数为num个，
%当num大于0小于等于1的时候表示求取的特征数的能量为num
%输出：basevector表示求取的最大特征值对应的特征向量，
      %newsample表示在basevector映射下获得的新特征矩阵。
      %returnsample表示用降维后特征恢复的特征矩阵
[u v]=size(patterns);%patterns实际为train_num*40行，112*92列
totalsamplemean=mean(patterns);%全体特征每一列的平均值
for i=1:u%对于训练集中每一个图片的行向量
    gensample(i,:)=patterns(i,:)-totalsamplemean;%对每个特征去平均值
end
sigma=gensample*gensample';%去均值后的协方差矩阵，变为边长为训练集个数的方阵
[U V]=eig(sigma);%特征值分解，构成对角阵V，对应特征向量构成U的列向量
d=diag(V);%提出所有特征值做成对角阵
[d1 index]=dsort(d);%降序排列特征值为d1,对应的原序列编号为index
if num>1%如果直接指定PCA降维中提取最大的几个特征值
    for i=1:num
        %提取出最大的num个协方差特征值对应的特征列向量，行数为样本数
        vector(:,i)=U(:,index(i));
        %num个原新特征值对应的特征向量，base有10304行num列
        base(:,i)=d(index(i))^(-1/2)* gensample' * vector(:,i);
    end
else%如果直接指定PCA降维中提取的能量
    sumv=sum(d1);%特征值总和
    for i=1:u%对于112*92个特征
        if sum(d1(1:i))/sumv>=num
            l=i;%看看前多少个特征占了num%的能量,剩下的丢弃
            break;
        end
    end
    for i=1:l
        %提取出最大的num个协方差特征值对应的特征列向量
        vector(:,i)=U(:,index(i));
        %num个原新特征值对应的特征向量
        base(:,i)=d(index(i))^(-1/2)* gensample' * vector(:,i);
    end
end
newsample=patterns*base;%拿特征向量矩阵乘以原特征矩阵得到新特征矩阵，有train行所需num列
basevector=base;%特征向量提取矩阵，有有10304行num列
origsample= (gensample*base)*base';
for i=1:u%对于训练集中每一个图片的行向量
    returnsample(i,:)=origsample(i,:)+totalsamplemean;%对每个特征去平均值
end