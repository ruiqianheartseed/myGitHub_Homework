function sample=Readsample(address,classnum,num)
%�������������ȡ������
%���룺address�����ĵ�ַ,classnum����Ҫ�������������,num��ÿ�����Ҫȡ����������
%���Ϊ��������
allsamples=[];
image=imread([pwd '\ORL\s1_1.bmp']);%�����һ��ͼ��
[rows cols]=size(image);%���ͼ���������������ʵ��Ϊ112*92
for i=classnum %��һ�˵���40��
    for j=num 
        a=imread(strcat(address,num2str(i),'_',num2str(j),'.bmp'));
        b=a(1:rows*cols);%ÿ��ͼƬ����Ϊ������
        b=double(b);%�ɻҶ�ֵת��Ϊdoubleģʽ���ڼ���
        allsamples=[allsamples;b];%һ��һ�����
    end
end
sample=allsamples;