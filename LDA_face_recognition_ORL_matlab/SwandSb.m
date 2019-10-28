function [sw sb]=SwandSb(samples,classnum,num)
%计算sw，sb
%samples为输入的降维后特征矩阵，classnum为类别数40，num为每一类的训练样本数
%输出sw为总的类内离散度矩阵，sb为类间协方差矩阵。
samplemean=mean(samples);
for i=1:classnum
    %计算降维后的每一类之内的特征样本均值
    newsamplemean(i,:)=mean(samples((i-1)*num+1:i*num,:));
end
sw=0;
for i=1:classnum
    for j=1:num%类内离散度矩阵
        sw=sw+(samples((i-1)*num+j,:)-newsamplemean(i,:))'*(samples((i-1)*num+j,:)-newsamplemean(i,:));
    end
end
sb=0;
for i=1:classnum%类间协方差矩阵
    sb=sb+(newsamplemean(i,:)-samplemean)'*(newsamplemean(i,:)-samplemean);
end