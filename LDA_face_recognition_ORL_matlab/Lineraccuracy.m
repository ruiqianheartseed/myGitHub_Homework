function accuracy=Lineraccuracy(testsample,num1,trainsample,num2)
%�����б�׼�����׼ȷ�ʵĺ���
%����testsample��ʾ����ͶӰ��Ĳ�������,num1��ʾÿһ����������ĸ���,
%trainsample������ͶӰ���ѵ������,num2����ÿһ��ѵ�������ĸ���
%���Ϊ��ȷ��
accu=0;
testsampnum=size(testsample,1);%���Ը���
trainsampnum=size(trainsample,1);%ѵ������
for i=1:testsampnum
    for j=1:trainsampnum
        %��ÿһ������������ѵ������֮����ͶӰ�����ռ��еľ���
        juli(j)=norm(testsample(i,:)-trainsample(j,:));
    end
    [temp index]=sort(juli);%����ѵ����������������
    if ceil(i/num1)==ceil(index(1)/num2)%�;���������Ǹ������Ƿ�����ͬһ��
        accu=accu+1;
    end
end
accuracy=accu/testsampnum;
