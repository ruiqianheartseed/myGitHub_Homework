function Showface(fea)
% ����һ�ж�Ӧһ��ͼ��ľ���
faceW = 92;%��92��112
faceH = 112;
numPerLine = 20;%һ��20��ͼ����ʾ6��
ShowLine = 6;
%������������
Y = zeros(faceH*ShowLine,faceW*numPerLine);
for i=0:ShowLine-1
   for j=0:numPerLine-1
     Y(i*faceH+1:(i+1)*faceH,j*faceW+1:(j+1)*faceW) = reshape(fea(i*numPerLine+j+1,:),[faceH,faceW]);
   end
end
figure;%������кõĻҶ�ͼ��
imagesc(Y);colormap(gray);axis image; axis off;
end