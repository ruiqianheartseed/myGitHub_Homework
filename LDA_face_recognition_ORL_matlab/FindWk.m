function [vsort,d1]=FindWk(sw,sb,num)
%����sw/sbͶӰ������������������
%swΪ����Э�������sb�����Э�������
%numΪ�����ȡ�õ�ά������
%vsortΪ����ͶӰ����
%invSw=inv(sw);%Sw^-1
newspace=sw\sb;%Sw^-1*Sb
[x,y]=eig(newspace);%��������ֵ�ֽ�,xΪ����������yΪ����ֵ
d=diag(y);%��ȡ����ֵ������ü�������w
[d1,index1]=dsort(d);
for i=1:num%��ȡ����fisherͶӰ����
    vsort(:,i)=x(:,index1(i));
end