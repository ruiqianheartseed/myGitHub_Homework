#include "stdafx.h"
#include "Morphology.h"

/***********************************************************************
* 函数名称：
* Morphology()
*
*说明：无参数的构造函数，对成员变量进行初始化
***********************************************************************/
Morphology::Morphology()
{

	m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
    m_nBitCountOut=0;//输出图像每像素位数为0	

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0

	m_maskBuf=NULL;//结构元素指针为空

	m_maskW=0;//结构元素宽为0

	m_maskH=0;//结构元素高为0
}

/***********************************************************************
* 函数名称：
* Morphology()
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
*      及位图数据，调用ImgCenterDib()对基类成员初始化，,并初始化派生类的
*      数据成员
***********************************************************************/
Morphology::Morphology(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					   unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{

	//输出图像格式与输入图像相同，此处直接在构造函数中分配输出图像
	//所需要的缓冲区，并填写输出图像的信息
    m_nBitCountOut=m_nBitCount;

	//输出图像颜色表处理
	m_lpColorTableOut=NULL;
	int colorTableLength=ComputeColorTabalLength(m_nBitCountOut);
	if(colorTableLength!=0){
    	m_lpColorTableOut=new RGBQUAD[colorTableLength];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*colorTableLength);
	}

	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像所需要的缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	m_maskBuf=NULL;//结构元素指针为空

	m_maskW=0;//结构元素宽为0

	m_maskH=0;//结构元素高为0

}

/***********************************************************************
* 函数名称：
*   ~Morphology()
*
*说明：析构函数，释放资源
***********************************************************************/
Morphology::~Morphology()
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

	//释放结构元素缓冲区
	if(m_maskBuf){
		delete []m_maskBuf;
		m_maskBuf=NULL;
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
CSize Morphology::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* 函数名称：
* InputMask()
*
*函数参数：
*  int *mask   -结构元素缓冲区指针
*  int maskW   -结构元素宽
*  int maskH   -结构元素高
*
*返回值：
*   无
*
*说明：结构元素输入函数，在调用其它函数前必须先调用此函数
***********************************************************************/
void Morphology::InputMask(int *mask,int maskW, int maskH)
{
	if(m_maskBuf){
		delete []m_maskBuf;
		m_maskBuf=NULL;
	}

	//将输入的结构元素拷贝到m_maskBuf缓冲区中
	m_maskBuf=new int[maskW*maskH];
	int i, j;
	for(i=0;i<maskH;i++){
		for(j=0;j<maskW;j++){
			m_maskBuf[i*maskW+j]=mask[i*maskW+j];
		}
	}

	//为结构元素宽和高赋值
	m_maskW=maskW;
	m_maskH=maskH;
}
/***********************************************************************
* 函数名称：
* GrayTopHatVally()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：波谷检测器，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayTopHatVally()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//每行图像数据的字节数,为4的倍数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//先膨胀后腐蚀为闭运算
	BasicDilationForGray(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicErosionForGray(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);


	//波谷检测器为闭运算减去原图的结果
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			*(m_pImgDataOut + i * lineByte + j) -= *(m_pImgData + i * lineByte + j);
		}
	}

	//释放缓冲区
	delete[]buf;
}

/***********************************************************************
* 函数名称：
* BasicDilationForGray()
*
*函数参数：
*  unsigned char *imgBufIn   -待腐蚀的图像
*  unsigned char *imgBufOut  -腐蚀后的结果
*  int imgWidth   -图像宽
*  int imgHeight   -图像高
*  int *maskBuf   -结构元素缓冲区指针
*  int maskW   -结构元素宽
*  int maskH   -结构元素高
*
*返回值：
*   无
*
*说明：灰值膨胀基本运算,后面的灰值开/闭/形态学梯度等操作都要调用这个函数
***********************************************************************/
void Morphology::BasicDilationForGray(unsigned char *imgBufIn,
	unsigned char *imgBufOut, int imgWidth, int imgHeight,
	int *maskBuf, int maskW, int maskH)
{
	//循环变量
	int i, j, k, l;

	//图像每行像素所占字节数
	int lineByte = (imgWidth + 3) / 4 * 4;
	int gray, tmp;

	//将输入图像,上下加m_maskH/2个像素的灰度为0的黑边，左右加m_maskW/2个像
//素的灰度为0的黑边，加边后的图像存入buf
	unsigned char *buf = new unsigned char[(imgHeight + maskH)*(lineByte + maskW)];
	for (i = 0; i < imgHeight + maskH; i++)
	{
		for (j = 0; j < lineByte + maskW; j++)
		{
			if (i < maskH / 2 || i >= imgHeight + maskH / 2 || j < maskW / 2 || j >= lineByte + maskW / 2)
				*(buf + i * (lineByte + maskW) + j) = 0;
			else
				*(buf + i * (lineByte + maskW) + j) = *(imgBufIn + (i - maskH / 2)*lineByte + j - maskW / 2);
		}
	}


	//膨胀运算
	for (i = maskH / 2; i < imgHeight + maskH / 2; i++)
	{
		for (j = maskW / 2; j < imgWidth + maskW / 2; j++)
		{
			gray = 0;
			for (k = -maskH / 2; k <= maskH / 2; k++)
			{
				for (l = -maskW / 2; l <= maskW / 2; l++)
				{
					if (maskBuf[(k + maskH / 2)*maskW + l + maskW / 2]) {
						tmp = *(buf + (i + k)*(lineByte + maskW) + j + l)
							+ maskBuf[(k + maskH / 2)*maskW + l + maskW / 2];
						if (tmp > gray)
							gray = tmp;
					}
				}
			}
			if (gray > 255)
				*(imgBufOut + (i - maskH / 2)*lineByte + j - maskW / 2) = 255;
			else
				*(imgBufOut + (i - maskH / 2)*lineByte + j - maskW / 2) = gray;
		}
	}

	//释放缓冲区
	delete[]buf;
}
/***********************************************************************
* 函数名称：
* BasicErosionForGray()
*
*函数参数：
*  unsigned char *imgBufIn   -待腐蚀的图像
*  unsigned char *imgBufOut  -腐蚀后的结果
*  int imgWidth   -图像宽
*  int imgHeight   -图像高
*  int *maskBuf   -结构元素缓冲区指针
*  int maskW   -结构元素宽
*  int maskH   -结构元素高
*
*返回值：
*   无
*
*说明：灰值腐蚀基本运算,后面的灰值开/闭/形态学梯度等操作都要调用这个函数
***********************************************************************/
void Morphology::BasicErosionForGray(unsigned char *imgBufIn,
	unsigned char *imgBufOut, int imgWidth, int imgHeight,
	int *maskBuf, int maskW, int maskH)
{
	//循环   变量
	int i, j, k, l;

	//灰度图像每行像素所占字节数
	int lineByte = (imgWidth + 3) / 4 * 4;

	//申请缓冲区存放中间结果
	unsigned char *buf = new unsigned char[(imgHeight + maskH)*(lineByte + maskW)];

	//将输入图像,上下加m_maskH/2个像素的灰度为255的亮边，左右加m_maskW/2个像
	//素的灰度为255的亮边，加边后的图像存入buf
	for (i = 0; i < imgHeight + maskH; i++)
	{
		for (j = 0; j < lineByte + maskW; j++)
		{
			if (i < maskH / 2 || i >= imgHeight + maskH / 2 || j < maskW / 2 || j >= lineByte + maskW / 2)
				*(buf + i * (lineByte + maskW) + j) = 255;
			else
				*(buf + i * (lineByte + maskW) + j) = *(imgBufIn + (i - maskH / 2)*lineByte + j - maskW / 2);
		}
	}

	//腐蚀运算，定义域内输入数据减对应模板数据，并求最小值，该最小值便是该点输出
	int gray, tmp;
	for (i = maskH / 2; i < imgHeight + maskH / 2; i++)
	{
		for (j = maskW / 2; j < lineByte + maskW / 2; j++)
		{
			gray = 255;
			for (k = -maskH / 2; k <= maskH / 2; k++)
			{
				for (l = -maskW / 2; l <= maskW / 2; l++)
				{
					if (maskBuf[(k + maskH / 2)*maskW + l + maskW / 2]) {
						tmp = *(buf + (i + k)*(lineByte + maskW) + j + l) -
							maskBuf[(k + maskH / 2)*maskW + l + maskW / 2];
						if (tmp < gray)
							gray = tmp;
					}
				}
			}
			if (gray < 0)
				*(imgBufOut + (i - maskH / 2)*lineByte + j - maskW / 2) = 0;
			else
				*(imgBufOut + (i - maskH / 2)*lineByte + j - maskW / 2) = gray;
		}
	}

	//释放缓冲区
	delete[]buf;
}








/***********************************************************************
* 函数名称：
* basicErosionForBinary()
*
*函数参数：
*  unsigned char *imgBufIn   -待腐蚀的图像
*  unsigned char *imgBufOut  -腐蚀后的结果
*  int imgWidth   -图像宽
*  int imgHeight   -图像高
*  int *maskBuf   -结构元素缓冲区指针
*  int maskW   -结构元素宽
*  int maskH   -结构元素高
*
*返回值：
*   无
*
*说明：二值腐蚀基本运算,背景为黑色,目标为白色,开/闭/二值形态学边界等操作
*      都要调用这个函数
***********************************************************************/
void Morphology::BasicErosionForBinary(unsigned char *imgBufIn,
	unsigned char *imgBufOut, int imgWidth, int imgHeight,
	int *maskBuf, int maskW, int maskH)
{
	//每行图像数据的字节数为4的倍数
	int lineByte = (imgWidth + 3) / 4 * 4;

	//把输入图像缓冲区的拷贝到输出图像缓冲区
	memcpy(imgBufOut, imgBufIn, lineByte*imgHeight);

	//定义变量
	int i, j, k, l;

	//标志变量
	int judge;

	//针对图像中每一个像素位置，判断是否结构元素能填入目标内部
	for (i = maskH / 2; i < imgHeight - maskH / 2; i++)
	{
		for (j = maskW / 2; j < imgWidth - maskW / 2; j++)
		{
			//判断结构元素是否可以在当前点填入目标内部,1为可以,0为不可以
			judge = 1;
			for (k = -maskH / 2; k <= maskH / 2; k++)
			{
				for (l = -maskW / 2; l <= maskW / 2; l++)
				{
					//如果当前结构元素位置为1,判断与对应图像上的像素点是否为非0
					if (maskBuf[(k + maskH / 2)*maskW + l + maskW / 2])
					{
						//如果图像当前象素为0,则没有击中该点,不是腐蚀的输出
						if (!*(imgBufIn + (i + k)*lineByte + j + l))
							judge = 0;
					}
				}
			}


			if (judge)
				*(imgBufOut + i * lineByte + j) = 255;
			else
				*(imgBufOut + i * lineByte + j) = 0;
		}
	}
}

/***********************************************************************
* 函数名称：
* BasicDilationForBinary()
*
*函数参数：
*  unsigned char *imgBufIn   -待膨胀的图像
*  unsigned char *imgBufOut  -膨胀后的结果
*  int imgWidth   -图像宽
*  int imgHeight   -图像高
*  int *maskBuf   -结构元素缓冲区指针
*  int maskW   -结构元素宽
*  int maskH   -结构元素高
*
*返回值：
*   无
*
*说明：二值膨胀基本运算,背景为黑色,目标为白色,开/闭/二值形态学边界等操作
*      都要调用这个函数
***********************************************************************/
void Morphology::BasicDilationForBinary(unsigned char *imgBufIn,
	unsigned char *imgBufOut, int imgWidth, int imgHeight,
	int *maskBuf, int maskW, int maskH)
{
	//每行图像数据的字节数为4的倍数
	int lineByte = (imgWidth + 3) / 4 * 4;

	//循环变量
	int i, j, k, l;

	//膨胀是对图像中目标补集的腐蚀，因此先求输入图像数据的补集
	for (i = 0; i < imgHeight; i++) {
		for (j = 0; j < imgWidth; j++) {
			*(imgBufIn + i * lineByte + j) = 255 - *(imgBufIn + i * lineByte + j);
		}
	}

	//膨胀是结构元素的对称集对补集的腐蚀,此处tempMask存放m_mask的对称集
	int *tempMask = new int[maskW*maskH];
	for (k = 0; k < maskH; k++) {
		for (l = 0; l < maskW; l++) {
			tempMask[k*maskW + l] = maskBuf[(maskH - 1 - k)*maskW + maskW - 1 - l];
		}
	}

	//标志变量
	int judge;

	//针对图像中每一个像素位置，判断是否结构元素能填入目标内部
	for (i = maskH / 2; i < imgHeight - maskH / 2; i++)
	{
		for (j = maskW / 2; j < imgWidth - maskW / 2; j++)
		{
			//判断结构元素是否可以在当前点填入目标内部,1为可以,0为不可以
			judge = 1;
			for (k = -maskH / 2; k <= maskH / 2; k++)
			{
				for (l = -maskW / 2; l <= maskW / 2; l++)
				{
					//如果当前结构元素位置为1,判断与对应图像上的像素点是否为非0
					if (tempMask[(k + maskH / 2)*maskW + l + maskW / 2])
					{
						if (!*(imgBufIn + (i + k)*lineByte + j + l))
							judge = 0;
					}
				}
			}


			if (judge)
				*(imgBufOut + i * lineByte + j) = 255;
			else
				*(imgBufOut + i * lineByte + j) = 0;
		}
	}

	//用结构元素对称集对目标补集腐蚀后,还要对结果再求一次补集,才是膨胀结果输出
	for (i = 0; i < imgHeight; i++) {
		for (j = 0; j < imgWidth; j++) {
			*(imgBufOut + i * lineByte + j) = 255 - *(imgBufOut + i * lineByte + j);
		}
	}

	//恢复原始数据
	for (i = 0; i < imgHeight; i++) {
		for (j = 0; j < imgWidth; j++) {
			*(imgBufIn + i * lineByte + j) = 255 - *(imgBufIn + i * lineByte + j);
		}
	}

	delete[]tempMask;
}

/***********************************************************************
* 函数名称：
* BinaryErosion()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：二值腐蚀，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::BinaryErosion()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//二值腐蚀
	BasicErosionForBinary(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

}

/***********************************************************************
* 函数名称：
* BinaryDilation()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：二值膨胀，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::BinaryDilation()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//二值膨胀
	BasicDilationForBinary(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
}


/***********************************************************************
* 函数名称：
* BinaryOpen()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：二值开运算，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::BinaryOpen()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//灰度图像每行像素所占字节数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//先腐蚀
	BasicErosionForBinary(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//后膨胀
	BasicDilationForBinary(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//释放缓冲区
	delete[]buf;

}


/***********************************************************************
* 函数名称：
* BinaryClose()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：二值闭运算，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::BinaryClose()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//灰度图像每行像素所占字节数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//先膨胀
	BasicDilationForBinary(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//后腐蚀
	BasicErosionForBinary(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//释放缓冲区
	delete[]buf;
}

/***********************************************************************
* 函数名称：
* BinaryInnerEdge()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：求二值形态学内边界，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::BinaryInnerEdge()
{
	//如果没有模板输入，则返回
	if (m_maskBuf == NULL)
		return;

	//灰度图像每行像素所占字节数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//腐蚀
	BasicErosionForBinary(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//原始数据减腐蚀结果
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < lineByte; j++) {
			*(m_pImgDataOut + i * lineByte + j) =
				*(m_pImgData + i * lineByte + j) - *(m_pImgDataOut + i * lineByte + j);
		}
	}

}

/***********************************************************************
* 函数名称：
* BinaryOuterEdge()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：求二值形态学外边界，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::BinaryOuterEdge()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//灰度图像每行像素所占字节数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//腐蚀
	BasicDilationForBinary(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//膨胀结果减去原始数据
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < lineByte; j++) {
			*(m_pImgDataOut + i * lineByte + j) -= *(m_pImgData + i * lineByte + j);
		}
	}
}

/***********************************************************************
* 函数名称：
* BinaryContour()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：求二值形态学边界，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::BinaryContour()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//灰度图像每行像素所占字节数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//腐蚀
	BasicErosionForBinary(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//膨胀
	BasicDilationForBinary(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//膨胀结果减腐蚀结果
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < lineByte; j++) {
			*(m_pImgDataOut + i * lineByte + j) -= *(buf + i * lineByte + j);
		}
	}

	//释放缓冲区
	delete[]buf;
}


/***********************************************************************
* 函数名称：
* HitAndMiss()
*
*函数参数：
*  unsigned char *imgBufIn   -待变换的输入图像
*  unsigned char *imgBufOut   -输出图像
*  int imgWidth   -图像宽
*  int imgHeight   -图像高
*  struct ElementPair hitMissMask  -击中击不中结构元素对
*
*返回值：
*   无
*
*说明：击中击不中变换,0代表背景,255代表目标
***********************************************************************/
void Morphology::HitAndMiss(unsigned char *imgBufIn, unsigned char *imgBufOut,
	int imgWidth, int imgHeight, ElementPair hitMissMask)
{
	//循环变量
	int i, j;

	//标志变量，1表示结构元素对有效，0表示无效
	int validateFlag = 1;

	//检查结构元素对是否交集为空,交集若不为空则为无效结构元素对,算法将退出
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (hitMissMask.hitElement[i * 3 + j] && hitMissMask.missElement[i * 3 + j]) {
				validateFlag = 0;
				break;
			}
		}
	}

	//非法结构元素对,返回
	if (validateFlag == 0)
		return;

	//图像每行像素所占字节数
	int lineByte = (imgWidth + 3) / 4 * 4;

	//输出图像置0
	memset(imgBufOut, 0, lineByte*imgHeight);

	//循环变量
	int k, l;

	//击中标志变量和击不中标志变量
	int hitFlag, missFlag;

	for (i = 1; i < imgHeight - 1; i++) {
		for (j = 1; j < imgWidth - 1; j++) {
			hitFlag = 1;
			missFlag = 1;
			for (k = -1; k <= 1; k++) {
				for (l = -1; l <= 1; l++) {
					//如果击中结构元素当前位置为1
					if (hitMissMask.hitElement[(k + 1) * 3 + l + 1] == 1) {
						//判断图像对应点是否为0,如果是,则没有击中图像当前点
						if (!*(imgBufIn + (i + k)*lineByte + j + l))
							hitFlag = 0;
					}

					//如果击不中结构元素当前位置为1
					if (hitMissMask.missElement[(k + 1) * 3 + l + 1] == 1) {
						//判断图像对应点是否为0,如果是,则没有击中图像当前点
						if (*(imgBufIn + (i + k)*lineByte + j + l))
							missFlag = 0;
					}
				}
			}

			//输出点置255
			if (hitFlag&&missFlag)
				*(imgBufOut + i * lineByte + j) = 255;
		}
	}
}

/***********************************************************************
* 函数名称：
* DefineElementPair()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：为击中击不中变换的结构元素对m_hitMissMask赋值，共8对
***********************************************************************/
void Morphology::DefineElementPair()
{
	//定义8个方向的击中击不中变换结构元素对
	int i, k, l;
	for (i = 0; i < 8; i++) {
		for (k = 0; k < 3; k++) {
			for (l = 0; l < 3; l++) {
				m_hitMissMask[i].hitElement[k * 3 + l] = 0;
				m_hitMissMask[i].missElement[k * 3 + l] = 0;
			}
		}
	}

	m_hitMissMask[0].hitElement[0] = 1;
	m_hitMissMask[0].hitElement[1] = 1;
	m_hitMissMask[0].hitElement[2] = 1;
	m_hitMissMask[0].hitElement[4] = 1;
	m_hitMissMask[0].missElement[6] = 1;
	m_hitMissMask[0].missElement[7] = 1;
	m_hitMissMask[0].missElement[8] = 1;

	m_hitMissMask[1].hitElement[6] = 1;
	m_hitMissMask[1].hitElement[7] = 1;
	m_hitMissMask[1].hitElement[8] = 1;
	m_hitMissMask[1].hitElement[4] = 1;
	m_hitMissMask[1].missElement[0] = 1;
	m_hitMissMask[1].missElement[1] = 1;
	m_hitMissMask[1].missElement[2] = 1;

	m_hitMissMask[2].hitElement[2] = 1;
	m_hitMissMask[2].hitElement[5] = 1;
	m_hitMissMask[2].hitElement[8] = 1;
	m_hitMissMask[2].hitElement[4] = 1;
	m_hitMissMask[2].missElement[0] = 1;
	m_hitMissMask[2].missElement[3] = 1;
	m_hitMissMask[2].missElement[6] = 1;

	m_hitMissMask[3].hitElement[0] = 1;
	m_hitMissMask[3].hitElement[3] = 1;
	m_hitMissMask[3].hitElement[6] = 1;
	m_hitMissMask[3].hitElement[4] = 1;
	m_hitMissMask[3].missElement[2] = 1;
	m_hitMissMask[3].missElement[5] = 1;
	m_hitMissMask[3].missElement[8] = 1;


	m_hitMissMask[4].hitElement[0] = 1;
	m_hitMissMask[4].hitElement[1] = 1;
	m_hitMissMask[4].hitElement[3] = 1;
	m_hitMissMask[4].hitElement[4] = 1;
	m_hitMissMask[4].missElement[5] = 1;
	m_hitMissMask[4].missElement[7] = 1;
	m_hitMissMask[4].missElement[8] = 1;

	m_hitMissMask[5].hitElement[5] = 1;
	m_hitMissMask[5].hitElement[7] = 1;
	m_hitMissMask[5].hitElement[8] = 1;
	m_hitMissMask[5].hitElement[4] = 1;
	m_hitMissMask[5].missElement[0] = 1;
	m_hitMissMask[5].missElement[1] = 1;
	m_hitMissMask[5].missElement[3] = 1;


	m_hitMissMask[6].hitElement[1] = 1;
	m_hitMissMask[6].hitElement[2] = 1;
	m_hitMissMask[6].hitElement[5] = 1;
	m_hitMissMask[6].hitElement[4] = 1;
	m_hitMissMask[6].missElement[3] = 1;
	m_hitMissMask[6].missElement[6] = 1;
	m_hitMissMask[6].missElement[7] = 1;

	m_hitMissMask[7].hitElement[3] = 1;
	m_hitMissMask[7].hitElement[6] = 1;
	m_hitMissMask[7].hitElement[7] = 1;
	m_hitMissMask[7].hitElement[4] = 1;
	m_hitMissMask[7].missElement[1] = 1;
	m_hitMissMask[7].missElement[2] = 1;
	m_hitMissMask[7].missElement[5] = 1;
}


/***********************************************************************
* 函数名称：
* MorphoThinning()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：用击中击不中变换对输入图像进行细化
***********************************************************************/
void Morphology::MorphoThinning()
{
	//定义8个方向的结构元素对
	DefineElementPair();


	//灰度图像每行像素所占字节数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//申请缓冲区，存放中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];
	memcpy(buf, m_pImgData, lineByte*m_imgHeight);

	//循环变量
	int i, k, l;

	//thinFlag用来记录当前一个循环周期内是否有细化掉的点,如果为0表明没
	//有可以细化的点存在,细化终止	
	int thinFlag = 1;
	while (thinFlag) {
		thinFlag = 0;
		//用8个方向的结构元素对都对图像作一次击中击不中变换,并将输出结果
		//从原图中去除
		for (i = 0; i < 8; i++) {
			HitAndMiss(buf, m_pImgDataOut, m_imgWidth, m_imgHeight, m_hitMissMask[i]);
			//将击中击不中变换的输出结果从原数据中去除
			for (k = 0; k < m_imgHeight; k++) {
				for (l = 0; l < lineByte; l++) {
					if (*(m_pImgDataOut + k * lineByte + l) == 255) {
						*(buf + k * lineByte + l) = 0;
						thinFlag = 1;
					}
				}
			}
		}
	}

	//把最后细化的结果拷贝到m_pImgDataOut
	memcpy(m_pImgDataOut, buf, lineByte*m_imgHeight);

	//释放缓冲区
	delete[]buf;

}




/***********************************************************************
* 函数名称：
* GrayErosion()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：灰值腐蚀，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayErosion()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//腐蚀
	BasicErosionForGray(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
}


/***********************************************************************
* 函数名称：
* GrayDilation()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：灰值膨胀，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayDilation()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//膨胀
	BasicDilationForGray(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

}

/***********************************************************************
* 函数名称：
* GrayOpen()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：灰值开运算，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayOpen()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//每行图像数据的字节数,为4的倍数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//腐蚀
	BasicErosionForGray(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//膨胀
	BasicDilationForGray(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//释放缓冲区
	delete[]buf;
}

/***********************************************************************
* 函数名称：
* GrayClose()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：灰值闭运算，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayClose()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//每行图像数据的字节数,为4的倍数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//膨胀
	BasicDilationForGray(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//腐蚀
	BasicErosionForGray(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//释放缓冲区
	delete[]buf;
}

/***********************************************************************
* 函数名称：
* GrayGradient()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：灰值形态学梯度，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayGradient()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//每行图像数据的字节数,为4的倍数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//腐蚀
	BasicErosionForGray(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//膨胀
	BasicDilationForGray(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//形态学梯度为膨胀的结果减腐蚀的结果
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			*(m_pImgDataOut + i * lineByte + j) -= *(buf + i * lineByte + j);
		}
	}

	//释放缓冲区
	delete[]buf;

}

/***********************************************************************
* 函数名称：
* GrayTopHatPeak()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：波峰检测器，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayTopHatPeak()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//每行图像数据的字节数,为4的倍数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//先腐蚀后膨胀为开运算
	BasicErosionForGray(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicDilationForGray(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//波峰检测器为原图减去开运算的结果
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			*(m_pImgDataOut + i * lineByte + j)
				= *(m_pImgData + i * lineByte + j) - *(m_pImgDataOut + i * lineByte + j);
		}
	}

	//释放缓冲区
	delete[]buf;

}



/***********************************************************************
* 函数名称：
* GrayTopHatPeakVally()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：峰谷检测器，m_pImgData为输入图像，m_pImgDataOut为输出图像
***********************************************************************/
void Morphology::GrayTopHatPeakVally()
{
	//如果没有结构元素输入，则返回
	if (m_maskBuf == NULL)
		return;

	//每行图像数据的字节数,为4的倍数
	int lineByte = (m_imgWidth + 3) / 4 * 4;

	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf = new unsigned char[lineByte*m_imgHeight];

	//先膨胀后腐蚀为闭运算,闭运算后m_pImgDataOut中存放了闭运算的结果
	BasicDilationForGray(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicErosionForGray(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	////中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *bufOpen = new unsigned char[lineByte*m_imgHeight];

	//先腐蚀后膨胀为开运算，开运算后bufOpen中存放了开运算的结果
	BasicErosionForGray(m_pImgData, buf, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicDilationForGray(buf, bufOpen, m_imgWidth, m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//峰谷检测器为闭运算减去开运算的结果
	int i, j;
	for (i = 0; i < m_imgHeight; i++) {
		for (j = 0; j < m_imgWidth; j++) {
			*(m_pImgDataOut + i * lineByte + j) -= *(bufOpen + i * lineByte + j);
		}
	}

	//释放缓冲区
	delete[]buf;
	delete[]bufOpen;
}


