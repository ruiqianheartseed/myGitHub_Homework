function [sw sb]=SwandSb(samples,classnum,num)
%����sw��sb
%samplesΪ����Ľ�ά����������classnumΪ�����40��numΪÿһ���ѵ��������
%���swΪ�ܵ�������ɢ�Ⱦ���sbΪ���Э�������
samplemean=mean(samples);
for i=1:classnum
    %���㽵ά���ÿһ��֮�ڵ�����������ֵ
    newsamplemean(i,:)=mean(samples((i-1)*num+1:i*num,:));
end
sw=0;
for i=1:classnum
    for j=1:num%������ɢ�Ⱦ���
        sw=sw+(samples((i-1)*num+j,:)-newsamplemean(i,:))'*(samples((i-1)*num+j,:)-newsamplemean(i,:));
    end
end
sb=0;
for i=1:classnum%���Э�������
    sb=sb+(newsamplemean(i,:)-samplemean)'*(newsamplemean(i,:)-samplemean);
end