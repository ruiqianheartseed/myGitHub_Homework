function [newsample,basevector,returnsample]=PCA(patterns,num)
%��������������patterns��ʾ�����ԭ��������numΪ���Ʊ�����
%��num����1��ʱ���ʾҪ���������Ϊnum����
%��num����0С�ڵ���1��ʱ���ʾ��ȡ��������������Ϊnum
%�����basevector��ʾ��ȡ���������ֵ��Ӧ������������
      %newsample��ʾ��basevectorӳ���»�õ�����������
      %returnsample��ʾ�ý�ά�������ָ�����������
[u v]=size(patterns);%patternsʵ��Ϊtrain_num*40�У�112*92��
totalsamplemean=mean(patterns);%ȫ������ÿһ�е�ƽ��ֵ
for i=1:u%����ѵ������ÿһ��ͼƬ��������
    gensample(i,:)=patterns(i,:)-totalsamplemean;%��ÿ������ȥƽ��ֵ
end
sigma=gensample*gensample';%ȥ��ֵ���Э������󣬱�Ϊ�߳�Ϊѵ���������ķ���
[U V]=eig(sigma);%����ֵ�ֽ⣬���ɶԽ���V����Ӧ������������U��������
d=diag(V);%�����������ֵ���ɶԽ���
[d1 index]=dsort(d);%������������ֵΪd1,��Ӧ��ԭ���б��Ϊindex
if num>1%���ֱ��ָ��PCA��ά����ȡ���ļ�������ֵ
    for i=1:num
        %��ȡ������num��Э��������ֵ��Ӧ������������������Ϊ������
        vector(:,i)=U(:,index(i));
        %num��ԭ������ֵ��Ӧ������������base��10304��num��
        base(:,i)=d(index(i))^(-1/2)* gensample' * vector(:,i);
    end
else%���ֱ��ָ��PCA��ά����ȡ������
    sumv=sum(d1);%����ֵ�ܺ�
    for i=1:u%����112*92������
        if sum(d1(1:i))/sumv>=num
            l=i;%����ǰ���ٸ�����ռ��num%������,ʣ�µĶ���
            break;
        end
    end
    for i=1:l
        %��ȡ������num��Э��������ֵ��Ӧ������������
        vector(:,i)=U(:,index(i));
        %num��ԭ������ֵ��Ӧ����������
        base(:,i)=d(index(i))^(-1/2)* gensample' * vector(:,i);
    end
end
newsample=patterns*base;%�����������������ԭ��������õ�������������train������num��
basevector=base;%����������ȡ��������10304��num��
origsample= (gensample*base)*base';
for i=1:u%����ѵ������ÿһ��ͼƬ��������
    returnsample(i,:)=origsample(i,:)+totalsamplemean;%��ÿ������ȥƽ��ֵ
end