function sample=Readsample(address,classnum,num)
%这个函数用来读取样本。
%输入：address样本的地址,classnum代表要读入样本的类别,num是每类的需要取出的样本；
%输出为样本矩阵
allsamples=[];
image=imread([pwd '\ORL\s1_1.bmp']);%读入第一幅图像
[rows cols]=size(image);%获得图像的行数和列数，实际为112*92
for i=classnum %第一人到第40人
    for j=num 
        a=imread(strcat(address,num2str(i),'_',num2str(j),'.bmp'));
        b=a(1:rows*cols);%每个图片变形为行向量
        b=double(b);%由灰度值转化为double模式便于计算
        allsamples=[allsamples;b];%一行一行添加
    end
end
sample=allsamples;