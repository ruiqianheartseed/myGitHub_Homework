function Showface(fea)
% 输入一行对应一个图像的矩阵
faceW = 92;%宽92高112
faceH = 112;
numPerLine = 20;%一行20个图，显示6行
ShowLine = 6;
%重新排列数据
Y = zeros(faceH*ShowLine,faceW*numPerLine);
for i=0:ShowLine-1
   for j=0:numPerLine-1
     Y(i*faceH+1:(i+1)*faceH,j*faceW+1:(j+1)*faceW) = reshape(fea(i*numPerLine+j+1,:),[faceH,faceW]);
   end
end
figure;%输出排列好的灰度图像
imagesc(Y);colormap(gray);axis image; axis off;
end