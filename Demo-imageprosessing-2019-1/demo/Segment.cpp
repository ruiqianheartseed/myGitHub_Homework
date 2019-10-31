#include "stdafx.h"
#include "Segment.h"
#include "math.h"

/***********************************************************************
* 函数名称：
* ImgSegment()
*
*说明：无参数的构造函数，对成员变量进行初始化
***********************************************************************/
ImgSegment::ImgSegment()
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0	

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}

/***********************************************************************
* 函数名称：
* ImgSegment()
*
*函数参数：
*  CSize size -图像大小（宽、高）
*  int nBitCount  -每像素所占位数
*  LPRGBQUAD lpColorTable  -颜色表指针
*  unsigned char *pImgData  -位图数据指针
*
*返回值：
*   无
*
*说明：本函数为带参数的构造函数，给定位图的大小、每像素位数、颜色表
*      及位图数据，调用ImgCenterDib()对基类成员初始化，并初始化派生类的
*      数据成员
***********************************************************************/
ImgSegment::ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}

/***********************************************************************
* 函数名称：
*   ~ImgSegment()
*
*说明：析构函数，释放资源
***********************************************************************/
ImgSegment::~ImgSegment()
{
	//释放输出图像位图数据缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//释放输出图像颜色表
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/***********************************************************************
* 函数名称：
* GetDimensions()
*
*函数参数：
*  无
*
*返回值：
*   图像的尺寸，用CSize类型表达
*
*说明：返回输出图像的宽和高
***********************************************************************/
CSize ImgSegment::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}
/***********************************************************************/
int ImgSegment::calculate(int histArray[256])
{

	//c0组产生的概率
	double w0 = 0;
	//c0组的像素数
	int count0 = 0;
	//阈值t
	int t = 0;
	int maxT = 0;
	//统计直方图中像素的个数，存放在sum中
	int sum = 0;
	for (int i = 0; i < 256; i++)
		sum = sum + histArray[i];
	
	while (w0 < 0.01 && t <= 255) {
		//计算阈值为t时，c0产生的概率
		count0 += histArray[t];
		w0 = double(count0) / double(sum);
		t += 1;
		}
	maxT = t;
	//返回最佳阈值
	return maxT;
}

/***********************************************************************
* 函数名称：
* HoughTransform()
*
*函数参数：
*  unsigned char *imgBinaryIn  -二值图像输入数据指针
*  int width  -图像的 宽
*  int height -图像的高
*  int *houghBuf  -Hough变换所需要的缓冲区指针
*  int  houghWidth  -Hough变换缓冲区的宽
*  int houghHeight  -Hough变换缓冲区的高
*  float radiusResolution  -Hough变换的极坐标半径的检测分辨率
*  float angleResolution  -Hough变换的角度检测分辨率 
*  float *radius  -用来返回Hough变换检测出来的最长直线的极半径
*  float *angle   -用来返回Hough变换检测出来的最长直线的角度

*返回值：
*   无
*
*说明：给定图像数据，通过线检测hough变换检测直线
***********************************************************************/
void ImgSegment::HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    float *radius, float *angle)
{
	//申请循环变量
	int i, j;

	//清理变换空间累加数组
	for(i=0;i<houghHeight;i++){
		for(j=0;j<houghWidth;j++){
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//循环变量
	int r, a;

	//中间变量
	float tempR, tempA; 

	//遍历图像数据
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			//出现一个目标点
			if(*(imgBinaryIn+i*width+j)==0){

				//a代表角度的循环变量，在变换空间累加数组的垂直方向上
				for(a=0;a<houghHeight;a++){

					//按照给定变换角度的分辨率，求取角度
					tempA=(a-houghHeight/2)*angleResolution;

					//根据当前遍历的角度及x,y值求取对应极半径
					tempR=(j-width/2)*cos(tempA*2*3.1415926/360)+(i-height/2)*sin(tempA*2*3.1415926/360);
					r=tempR/radiusResolution;

					//累加数组累加
					*(houghBuf+a*houghWidth+ r+houghWidth/2)+=1;
				}
			}
		}
	}

	//求累加数组的极大值，并记录此时的数组坐标
	int max, maxR, maxA;
	max=*(houghBuf+0*houghWidth+0);
	maxR=0;
	maxA=0;
	for(a=0;a<houghHeight;a++){
		for(r=0;r<houghWidth;r++){
			if(max<=*(houghBuf+a*houghWidth+r)){
				max=*(houghBuf+a*houghWidth+r);
				maxR=r;
				maxA=a;
			}
		}
	}

	//将极大值位置转换成极坐标半径和角度,并通过参数返回
	*radius=(maxR-houghWidth/2)*radiusResolution;
	*angle=(maxA-houghHeight/2)*angleResolution;
}


/***********************************************************************
* 函数名称：
* Hough()
*
*函数参数：
*  float radiusResolution  -Hough变换的极坐标半径的检测分辨率
*  float angleResolution  -Hough变换的角度检测分辨率 
*  float *radius  -用来返回Hough变换检测出来的最长直线的极半径
*  float *angle   -用来返回Hough变换检测出来的最长直线的角度

*返回值：
*   无
*
*说明：给定Hough变换的极半径分辨率和角度分辨率，通过调用HoughTransform()
*      对输入图像m_pImgData做线检测，Hough变换的结果输出到m_pImgDataOut中
***********************************************************************/
void ImgSegment::Hough(float radiusResolution, float angleResolution,
					   float *radius, float *angle)
{
	//只处理灰度图像
	if(m_nBitCount!=8)
		return;

	//释放缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像的每像素位数及颜色表长度
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

	//图像的中心为坐标原点,线与原点的距离最大
	//为sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,所以Hough变换
	//的高度为：
	int houghWidth=sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight);
	 houghWidth /= radiusResolution;

	//线的角度在[-90,90]之间,所以申请的累加数组高度为181/angleResolution
	int houghHeight=181/angleResolution;

	//申请累加数组缓冲区
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough变换，结果存入houghBuf中
	HoughTransform(m_pImgData, m_imgWidth, m_imgHeight, houghBuf,  houghWidth,
	        	houghHeight,radiusResolution, angleResolution, radius, angle);
	
	//输出图像的大小是Hough变换累加数组的大小			
	m_imgWidthOut=houghWidth;
	m_imgHeightOut=houghHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//求出累加数组的最大值
	int i, j, max=0; 
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			if(max<=*(houghBuf+i*houghWidth+j))
				max=*(houghBuf+i*houghWidth+j);
		}
	}
	//根据最大值将累加数组映射到输出图像
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			*(m_pImgDataOut+i*lineByteOut+j)=
				*(houghBuf+i*houghWidth+j)*255.0/max;
		}
	}
	
	//释放缓冲区
	delete []houghBuf;
}

/***********************************************************************
* 函数名称：
* longestLineDetectByHough()
*
*函数参数：
*  float radiusResolution  -Hough变换的极坐标半径的检测分辨率
*  float angleResolution  -Hough变换的角度检测分辨率 
*  float *radius  -用来返回Hough变换检测出来的最长直线的极半径
*  float *angle   -用来返回Hough变换检测出来的最长直线的角度

*返回值：
*   无
*
*说明：给定Hough变换的极半径分辨率和角度分辨率，通过调用HoughTransform()
*      对输入图像m_pImgData做线检测，根据最长线的角度和极坐标半径,将最
*     长线输出到m_pImgDataOut中
***********************************************************************/
void ImgSegment::longestLineDetectByHough(float radiusResolution, float angleResolution)
{
	//只处理灰度图像
	if(m_nBitCount!=8)
		return;

	//释放缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像的每像素位数及颜色表长度
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

	//图像的中心为坐标原点,线与原点的距离最大
	//为sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,所以Hough变换
	//的高度为：
	int houghWidth=sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight);
	 houghWidth /= radiusResolution;

	//线的角度在[-90,90]之间,所以申请的累加数组高度为181/angleResolution
	int houghHeight=181/angleResolution;

	//申请累加数组缓冲区
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough变换，结果存入houghBuf中
	float radius, angle;
	HoughTransform(m_pImgData, m_imgWidth, m_imgHeight, houghBuf,  houghWidth,
	        	houghHeight,radiusResolution, angleResolution, &radius, &angle);
	float a = radius;
	float b = angle;
	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//根据求得的角度和极坐标半径,在白色背景下画出最长直线并作为输出图像显示
	memset(m_pImgDataOut,255,lineByteOut*m_imgHeightOut);

	//angle的单位是度,此处转换为弧度进行计算
	float alfa=angle*2*3.1415926/360;

	
	int x, y, i, j;
	for( x=-m_imgWidthOut/2;x<m_imgWidthOut/2;x++){
		//图像的中心位置为坐标原点,(x,y)为直角坐标系中的点,
		//将其转换至坐标原点在图像左下角的坐标系(i,j)
		y=radius/sin(alfa)-x/tan(alfa);
		j=x-m_imgWidthOut/2;
		i=y+m_imgHeightOut/2;
		if(i>0&&i<m_imgHeightOut)
	    	*(m_pImgDataOut+i*m_imgWidthOut+j)=0;
	}
	
	CString str1; // 这是MFC中的字符串变量
	CString str2; 
	CString str3; 
	CString str4; 
	CString str;
	str1.Format("直线的极坐标半径为： ");
	str2.Format("%f", a); // 调用Format方法将变量转换成字符串，第一个参数就是变量类型
	str3.Format("\n直线的角度为： ");
	str4.Format("%f", b);
	str = str1 + str2 + str3 + str4;
	::MessageBox(0, str, MB_OK, 0);
	//空间释放
	delete []houghBuf;
}


/***********************************************************************
* 函数名称：
* threshOtus()
*
*函数参数：
*  int histArray[256]  -图像的统计直方图
*
*  返回值：
*     最佳阈值
*
*说明：大津阈值选择函数，给定直方图数组，根据方差最大原理自动选择阈值，
*      对于彩色图像，该函数根据亮度直方图计算阈值
***********************************************************************/
int ImgSegment::threshOtus(int histArray[256])
{
	//c0和c1组的均值
	float u0, u1;

	//c0和c1组产生的概率
	float w0, w1;

	//c0组的像素数
	int count0;

	//阈值t及记录方差最大时的最佳阈值maxT
	int t, maxT;

	//方差及最大方差
	float devi, maxDevi = 0;

	//循环变量
	int i;

	//统计直方图中像素的个数，存放在sum中
	int sum = 0;
	for (i = 0; i < 256; i++)
		sum = sum + histArray[i];

	for (t = 0; t < 255; t++) {
		//计算阈值为t时，c0组的均值和产生的概率
		u0 = 0;
		count0 = 0;
		for (i = 0; i <= t; i++) {
			u0 += i * histArray[i];
			count0 += histArray[i];
		}
		u0 = u0 / count0;
		w0 = (float)count0 / sum;

		//计算阈值为t时，c1组的均值和产生的概率
		u1 = 0;
		for (i = t + 1; i < 256; i++)
			u1 += i * histArray[i];
		//C0组像素数与C1组像素数之和为图像总像素数。
		u1 = u1 / (sum - count0);
		w1 = 1 - w0;

		//两组间的方差
		devi = w0 * w1*(u1 - u0)*(u1 - u0);

		//记录最大的方差及最佳阈值位置
		if (devi > maxDevi) {
			maxDevi = devi;
			maxT = t;
		}

	}

	//返回最佳阈值
	return maxT;

}


/***********************************************************************
* 函数名称：
* Roberts()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Roberts边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Roberts()
{
	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i, j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte = m_nBitCount / 8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int x, y, t;

	//Roberts算子
	for (i = 1; i < m_imgHeight - 1; i++) {
		for (j = 1; j < m_imgWidth - 1; j++) {
			for (k = 0; k < pixelByte; k++) {
				//x方向梯度
				x = *(m_pImgData + i * lineByte + j * pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + j * pixelByte + k);

				//y方向梯度
				y = *(m_pImgData + i * lineByte + j * pixelByte + k)
					- *(m_pImgData + i * lineByte + (j + 1)*pixelByte + k);

				t = sqrt(x*x + y * y) + 0.5;
				if (t > 255)
					t = 255;
				*(m_pImgDataOut + i * lineByte + j * pixelByte + k) = t;
			}
		}
	}
}

/***********************************************************************
* 函数名称：
* Sobel()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Sobel边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Sobel()
{
	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i, j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte = m_nBitCount / 8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int x, y, t;

	//Sobel算子
	for (i = 1; i < m_imgHeight - 1; i++) {
		for (j = 1; j < m_imgWidth - 1; j++) {
			for (k = 0; k < pixelByte; k++) {
				//x方向梯度
				x = *(m_pImgData + (i - 1)*lineByte + (j + 1)*pixelByte + k)
					+ 2 * *(m_pImgData + i * lineByte + (j + 1)*pixelByte + k)
					+ *(m_pImgData + (i + 1)*lineByte + (j + 1)*pixelByte + k)
					- *(m_pImgData + (i - 1)*lineByte + (j - 1)*pixelByte + k)
					- 2 * *(m_pImgData + i * lineByte + (j - 1)*pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + (j - 1)*pixelByte + k);

				//y方向梯度
				y = *(m_pImgData + (i - 1)*lineByte + (j - 1)*pixelByte + k)
					+ 2 * *(m_pImgData + (i - 1)*lineByte + j * pixelByte + k)
					+ *(m_pImgData + (i - 1)*lineByte + (j + 1)*pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + (j - 1)*pixelByte + k)
					- 2 * *(m_pImgData + (i + 1)*lineByte + j * pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + (j + 1)*pixelByte + k);

				t = sqrt(x*x + y * y) + 0.5;
				if (t > 255)
					t = 255;
				*(m_pImgDataOut + i * lineByte + j * pixelByte + k) = t;
			}
		}
	}

}

/***********************************************************************
* 函数名称：
* Prewitt()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Prewitt边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Prewitt()
{
	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i, j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte = m_nBitCount / 8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int x, y, t;

	//Prewitt算子
	for (i = 1; i < m_imgHeight - 1; i++) {
		for (j = 1; j < m_imgWidth - 1; j++) {
			for (k = 0; k < pixelByte; k++) {
				//x方向梯度
				x = *(m_pImgData + (i - 1)*lineByte + (j + 1)*pixelByte + k)
					+ *(m_pImgData + i * lineByte + (j + 1)*pixelByte + k)
					+ *(m_pImgData + (i + 1)*lineByte + (j + 1)*pixelByte + k)
					- *(m_pImgData + (i - 1)*lineByte + (j - 1)*pixelByte + k)
					- *(m_pImgData + i * lineByte + (j - 1)*pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + (j - 1)*pixelByte + k);

				//y方向梯度
				y = *(m_pImgData + (i - 1)*lineByte + (j - 1)*pixelByte + k)
					+ *(m_pImgData + (i - 1)*lineByte + j * pixelByte + k)
					+ *(m_pImgData + (i - 1)*lineByte + (j + 1)*pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + (j - 1)*pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + j * pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + (j + 1)*pixelByte + k);

				t = sqrt(x*x + y * y) + 0.5;
				if (t > 255)
					t = 255;
				*(m_pImgDataOut + i * lineByte + j * pixelByte + k) = t;
			}
		}
	}

}


/***********************************************************************
* 函数名称：
* Laplacian()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Laplacian边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Laplacian()
{
	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i, j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte = m_nBitCount / 8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int t;

	//Laplacian算子
	for (i = 1; i < m_imgHeight - 1; i++) {
		for (j = 1; j < m_imgWidth - 1; j++) {
			for (k = 0; k < pixelByte; k++) {
				t = 4 * *(m_pImgData + i * lineByte + j * pixelByte + k)
					- *(m_pImgData + (i - 1)*lineByte + j * pixelByte + k)
					- *(m_pImgData + (i + 1)*lineByte + j * pixelByte + k)
					- *(m_pImgData + i * lineByte + (j - 1)*pixelByte + k)
					- *(m_pImgData + i * lineByte + (j + 1)*pixelByte + k);

				t = abs(t) + 0.5;
				if (t > 255)
					t = 255;
				*(m_pImgDataOut + i * lineByte + j * pixelByte + k) = t;
			}
		}
	}
}

/*************************************************************************
 *
 * 函数名称：
 *   TemplateEdge()
 *
 * 参数:
 *   unsigned char *imgIn	- 输入图像数据指针
 *   int width, int height  - 图像的宽和高（像素为单位）
 *   int nBitCount  -图像每像素位数
 *   int *mask -  模板指针
 *   int maskW, int maskH  - 模板的宽和高
 *   unsigned char *imgOut - 图像与模板卷积后（边缘检测）的输出
 *
 * 返回值:
 *   无
 *
 * 说明:
 *   模板卷积函数该函数用给定的mask模板与输入图像进行卷积,函数将灰度和彩色
 *   看作不同通道数据的合成，分别对不同通道数据进行模板卷积，从而完成灰度
 *   和彩色图像的边缘检测
 ************************************************************************/
void ImgSegment::TemplateEdge(unsigned char *imgIn, int width, int height, int nBitCount,
	int *mask, int maskW, int maskH, unsigned char *imgOut)
{
	//每行像素字节数
	int lineByte = (width*nBitCount / 8 + 3) / 4 * 4;

	//循环变量，图像的坐标
	int i, j;

	//每像素占字节数
	int pixelByte = nBitCount / 8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//循环变量，用于模板卷积运算
	int  c, r;

	//中间变量
	int sum;

	//模板卷积运算，边缘像素不处理
	for (i = maskH / 2; i < height - maskH / 2; i++) {
		for (j = maskW / 2; j < width - maskH / 2; j++) {
			//对每个通道数据进行卷积
			for (k = 0; k < pixelByte; k++) {
				//卷积求和
				sum = 0;
				for (r = -maskH / 2; r <= maskH / 2; r++) {
					for (c = -maskW / 2; c <= maskW / 2; c++) {
						sum += *(mask + (r + maskH / 2)*maskW + c + maskW / 2)
							* *(imgIn + (i + r)*lineByte + (j + c)*pixelByte + k);
					}
				}

				//取绝对值
				sum = abs(sum);
				if (sum > 255)
					sum = 255;
				else if (sum < 0)
					sum = 0;

				*(imgOut + i * lineByte + j * pixelByte + k) = sum;
			}
		}
	}
}


/***********************************************************************
* 函数名称：
* EdgeByAnyMask()
*
*函数参数：
*  int *mask   -二维的模板
*  int maskW   -模板宽
*  int maskH   -模板高
*
*返回值：
*   无
*
*说明：根据自定义模板检测边缘，同时适合于灰度和彩色图像
***********************************************************************/
void ImgSegment::EdgeByAnyMask(int *mask, int maskW, int maskH)
{
	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, maskW, maskH, m_pImgDataOut);
}

/***********************************************************************
* 函数名称：
* Krisch()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Krisch边缘检测，同时适合于灰度和彩色图像
***********************************************************************/
void ImgSegment::Krisch()
{
	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];

	//输出图像缓冲区初始化为0
	memset(m_pImgDataOut, 0, lineByte*m_imgHeight);

	//循环变量，图像的坐标
	int i, j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte = m_nBitCount / 8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//两个指针变量
	unsigned  char *p1, *p2;
	//申请临时缓冲区,存放中间结果
	unsigned char *buf = new unsigned char[lineByte* m_imgHeight];

	//模板数组
	int mask[9];

	// 设置Kirsch模板1参数
	mask[0] = 5;
	mask[1] = 5;
	mask[2] = 5;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = -3;

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				p1 = m_pImgDataOut + i * lineByte + j * pixelByte + k;
				p2 = buf + i * lineByte + j * pixelByte + k;
				if (*p1 < *p2)
					*p1 = *p2;
			}
		}
	}

	// 设置Kirsch模板2参数
	mask[0] = -3;
	mask[1] = 5;
	mask[2] = 5;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = 5;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = -3;

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				p1 = m_pImgDataOut + i * lineByte + j * pixelByte + k;
				p2 = buf + i * lineByte + j * pixelByte + k;
				if (*p1 < *p2)
					*p1 = *p2;
			}
		}
	}

	// 设置Kirsch模板3参数
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = 5;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = 5;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = 5;

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				p1 = m_pImgDataOut + i * lineByte + j * pixelByte + k;
				p2 = buf + i * lineByte + j * pixelByte + k;
				if (*p1 < *p2)
					*p1 = *p2;
			}
		}
	}


	// 设置Kirsch模板4参数
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = 5;
	mask[6] = -3;
	mask[7] = 5;
	mask[8] = 5;

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				p1 = m_pImgDataOut + i * lineByte + j * pixelByte + k;
				p2 = buf + i * lineByte + j * pixelByte + k;
				if (*p1 < *p2)
					*p1 = *p2;
			}
		}
	}

	// 设置Kirsch模板5参数
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = 5;
	mask[7] = 5;
	mask[8] = 5;

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				p1 = m_pImgDataOut + i * lineByte + j * pixelByte + k;
				p2 = buf + i * lineByte + j * pixelByte + k;
				if (*p1 < *p2)
					*p1 = *p2;
			}
		}
	}

	// 设置Kirsch模板6参数
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = 5;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = 5;
	mask[7] = 5;
	mask[8] = -3;

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				p1 = m_pImgDataOut + i * lineByte + j * pixelByte + k;
				p2 = buf + i * lineByte + j * pixelByte + k;
				if (*p1 < *p2)
					*p1 = *p2;
			}
		}
	}

	// 设置Kirsch模板7参数
	mask[0] = 5;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = 5;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = 5;
	mask[7] = -3;
	mask[8] = -3;

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				p1 = m_pImgDataOut + i * lineByte + j * pixelByte + k;
				p2 = buf + i * lineByte + j * pixelByte + k;
				if (*p1 < *p2)
					*p1 = *p2;
			}
		}
	}

	// 设置Kirsch模板8参数
	mask[0] = 5;
	mask[1] = 5;
	mask[2] = -3;
	mask[3] = 5;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = -3;
	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		mask, 3, 3, buf);

	//求两幅缓存图像的最大值,并将大的值存入m_pImgDataOut中
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			for (k = 0; k < pixelByte; k++) {
				if (*(m_pImgDataOut + i * lineByte + j * pixelByte + k) < *(buf + i * lineByte + j * pixelByte + k))
					*(m_pImgDataOut + i * lineByte + j * pixelByte + k) = *(buf + i * lineByte + j * pixelByte + k);
			}
		}
	}
}

/***********************************************************************
* 函数名称：
* GaussLaplacian()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Gauss-Laplacian边缘检测，同时适合于灰度和彩色图像
***********************************************************************/
void ImgSegment::GaussLaplacian()
{
	//定义Gauss-Laplacian模板
	int mask[25];
	mask[0] = -2;
	mask[1] = -4;
	mask[2] = -4;
	mask[3] = -4;
	mask[4] = -2;
	mask[5] = -4;
	mask[6] = 0;
	mask[7] = 8;
	mask[8] = 0;
	mask[9] = -4;
	mask[10] = -4;
	mask[11] = 8;
	mask[12] = 24;
	mask[13] = 8;
	mask[14] = -4;
	mask[15] = -4;
	mask[16] = 0;
	mask[17] = 8;
	mask[18] = 0;
	mask[19] = -4;
	mask[20] = -2;
	mask[21] = -4;
	mask[22] = -4;
	mask[23] = -4;
	mask[24] = -2;

	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];

	//模板卷积函数调用
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, mask, 5, 5, m_pImgDataOut);

}




/***********************************************************************
* 函数名称：
* RegionGrow()
*
*函数参数：
*  CPoint seed  -种子点
*  int thresh   -阈值
*
*返回值：
*   无
*
*说明：区域生长算法，给定一个种子点seed和阈值thresh，从种子点处开始生长，
*      将与种子点像素灰度值之差thresh的像素合并在一起形成一个区域，该函
*      数只处理灰度图像
***********************************************************************/
void ImgSegment::RegionGrow(CPoint seed, int thresh)
{
	//只处理灰度图像
	if (m_nBitCount != 8)
		return;

	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];

	//以下是区域生长代码

	//循环变量
	int i, j;

	//将输出图像初始化置255,用0代表像素的生长标记
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			*(m_pImgDataOut + i * lineByte + j) = 255;
		}
	}

	//二维数组direction代表中心像素点8邻域坐标与该点在x和y方向上的偏移,
	//其中第一列为x方向的偏移,第二列为y方向的偏移
	int direction[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };

	//栈申请，此处假定进栈的像素最多为图像总像素数
	CPoint *stack = new CPoint[m_imgWidth*m_imgHeight];

	//栈顶指针
	int top;

	//当前正处理的点和弹出的点
	CPoint currentPoint, popPoint;

	//循环变量，遍历array数组的第一维下标
	int k;

	//标记变量
	int label;

	//临时变量
	int temp1, temp2;

	//记录种子像素的灰度值
	temp1 = *(m_pImgData + seed.y*lineByte + seed.x);

	//将给定种子点置标记0,入栈
	*(m_pImgDataOut + seed.y*lineByte + seed.x) = 0;
	top = 0;
	stack[top].x = seed.x;
	stack[top].y = seed.y;

	//堆栈
	while (top > -1) {
		//弹出栈顶元素,该元素已经生长过
		popPoint.x = stack[top].x;
		popPoint.y = stack[top].y;
		top--;

		//考察弹出像素周围是否有没有生长的像素
		for (k = 0; k < 8; k++) {

			//待考察的邻域点
			currentPoint.x = popPoint.x + direction[k][0];
			currentPoint.y = popPoint.y + direction[k][1];

			//如果待考察的点不在图像内，则跳过
			if (currentPoint.x<0 || currentPoint.x>m_imgWidth - 1 ||
				currentPoint.y<0 || currentPoint.y>m_imgHeight - 1)
				continue;

			//该点标号
			label = *(m_pImgDataOut + currentPoint.y*lineByte + currentPoint.x);

			//弹出的点周围有尚没生长的点
			if (label == 255) {
				temp2 = *(m_pImgData + currentPoint.y*lineByte + currentPoint.x);
				//如果当前被考察的像素灰度值与种子点灰度值之差小于给定的阈值,
				//则认为相似,将其进栈处理
				if (abs(temp1 - temp2) < thresh) {
					//给该点置生长标记0
					*(m_pImgDataOut + currentPoint.y*lineByte + currentPoint.x) = 0;
					top++;
					stack[top].x = currentPoint.x;
					stack[top].y = currentPoint.y;
				}
			}
		}

	}

	//清除缓冲区
	delete[]stack;
}



/***********************************************************************
* 函数名称：
* ContourExtract()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：给定一个二值图像，把黑色区域看作目标，白色看作背景，提取目标的轮廓
***********************************************************************/
void ImgSegment::ContourExtract()
{
	//只处理灰度图像
	if (m_nBitCount != 8)
		return;

	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];

	//以下是轮廓提取代码

	//将输入图像数据拷贝置输出图像缓冲区
	memcpy(m_pImgDataOut, m_pImgData, lineByte*m_imgHeight);

	//存放黑点像素8邻域的像素灰度值
	int array[8];

	//数组求和
	int sum;

	//循环变量
	int i, j, k;

	//搜索图像中的黑点，不考虑边界上的点
	for (i = 1; i < m_imgHeight - 1; i++) {
		for (j = 1; j < m_imgWidth - 1; j++) {
			//找到一个黑点
			if (*(m_pImgData + i * lineByte + j) == 0) {
				//拷贝周围8邻域像素进array数组
				array[0] = *(m_pImgData + i * lineByte + j + 1);
				array[1] = *(m_pImgData + (i + 1)*lineByte + j + 1);
				array[2] = *(m_pImgData + (i + 1)*lineByte + j);
				array[3] = *(m_pImgData + (i + 1)*lineByte + j - 1);
				array[4] = *(m_pImgData + i * lineByte + j - 1);
				array[5] = *(m_pImgData + (i - 1)*lineByte + j - 1);
				array[6] = *(m_pImgData + (i - 1)*lineByte + j);
				array[7] = *(m_pImgData + (i - 1)*lineByte + j + 1);

				//对数组求和
				sum = 0;
				for (k = 0; k < 8; k++)
					sum += array[k];

				//周围8邻域均为黑点，则输出图像对应像素置白色
				if (sum == 0)
					*(m_pImgDataOut + i * lineByte + j) = 255;

			}
		}
	}

}

/***********************************************************************
* 函数名称：
* ContourTrace()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：给定一个二值图像，把黑色区域看作目标，白色看作背景，跟踪目标的边界
***********************************************************************/
void ImgSegment::ContourTrace()
{
	//只处理灰度图像
	if (m_nBitCount != 8)
		return;

	//释放m_pImgDataOut指向的图像数据空间
	if (m_pImgDataOut != NULL) {
		delete[]m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//释放颜色表空间
	if (m_lpColorTableOut != NULL) {
		delete[]m_lpColorTableOut;
		m_lpColorTableOut = NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut = m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut = ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if (m_nColorTableLengthOut != 0) {
		m_lpColorTableOut = new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut, m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut = m_imgWidth;
	m_imgHeightOut = m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (m_imgWidth*m_nBitCount / 8 + 3) / 4 * 4;

	//申请输出图像缓冲区
	m_pImgDataOut = new unsigned char[lineByte*m_imgHeight];

	//将输出图像初始化置255,用0代表轮廓像素标记
	memset(m_pImgDataOut, 255, lineByte*m_imgHeight);

	//顺时针定义中心像素点的8邻域坐标，第一列为x方向的偏移,第二列为y方向的偏移
	int direction[8][2] = { {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };

	//边界起始点,待处理的当前点,当前点的邻域点
	CPoint startP, currentP, neighborP;

	//是否当前点与起始边界点重合的标志变量
	int findStartPoint;

	//搜索边界起始点
	findStartPoint = 0;

	//循环变量，图像坐标
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			//找到起始边界点
			if (*(m_pImgData + i * lineByte + j) == 0) {
				startP.x = j;
				startP.y = i;
				*(m_pImgDataOut + i * lineByte + j) = 0;
				findStartPoint = 1;
				break;
			}
		}

		//已经找到起始边界点
		if (findStartPoint)
			break;
	}

	//边界跟踪
	//从初始点开始跟踪
	currentP.x = startP.x;
	currentP.y = startP.y;

	//邻域点是否边界点标志变量
	int isContourP;

	//开始方向
	int startDirect = 0;

	//0表示还没有返回最初的边界起始点
	findStartPoint = 0;
	while (findStartPoint == 0) {
		isContourP = false;
		while (isContourP == false) {
			neighborP.x = currentP.x + direction[startDirect][0];
			neighborP.y = currentP.y + direction[startDirect][1];

			//搜索到邻域点
			if (*(m_pImgData + neighborP.y*lineByte + neighborP.x) == 0) {
				isContourP = true;
				currentP.x = neighborP.x;
				currentP.y = neighborP.y;

				if (currentP.x == startP.x&&currentP.y == startP.y)
					findStartPoint = true;//回到边界起始点了

				*(m_pImgDataOut + currentP.y*lineByte + currentP.x) = 0;

				//扫描方向逆时针旋转90度
				startDirect -= 2;
				if (startDirect < 0)
					startDirect += 8;
			}
			else {
				//扫描方向顺时针旋转45度
				startDirect++;
				if (startDirect == 8)
					startDirect = 0;
			}

		}
	}

}







