function [vsort,d1]=FindWk(sw,sb,num)
%计算sw/sb投影的最优特征向量矩阵
%sw为类内协方差矩阵，sb是类间协方差矩阵
%num为最后想取得的维度数量
%vsort为最优投影向量
%invSw=inv(sw);%Sw^-1
newspace=sw\sb;%Sw^-1*Sb
[x,y]=eig(newspace);%进行特征值分解,x为特征向量，y为特征值
d=diag(y);%提取特征值，想求得鉴别向量w
[d1,index1]=dsort(d);
for i=1:num%提取出的fisher投影矩阵
    vsort(:,i)=x(:,index1(i));
end