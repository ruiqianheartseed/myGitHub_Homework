#ifndef _INSIDE_VISUAL_CPP_histogram
#define _INSIDE_VISUAL_CPP_histogram
#include "ImageCenterDib.h"

//直方图类
class Histogram:public ImgCenterDib
{
public:
	//直方图数组
	int m_histArray[256];

	//直方图平均值
	float m_average;

	//直方图方差
	float m_deviation;

	//输出图像每像素位数
	int m_nBitCountOut;

	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;

	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;
	
private:
	//输出图像的宽，像素为单位
	int m_imgWidthOut;

	//输出图像的高，像素为单位
	int m_imgHeightOut;

	//输出图像颜色表长度
	int m_nColorTableLengthOut;

public:
	//不带参数的构造函数
	Histogram();

	//带参数的构造函数
	Histogram(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);

	//析构函数
	~Histogram();

	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();

	//统计直方图
	void computeHistGray();
	//彩色图像亮度直方图
	void computeHistBrightness();

	//彩色图像红色通道直方图
	void computeHistRed();

	//彩色图像绿色通道直方图
	void computeHistGreen();

	//彩色图像兰色通道直方图
	void computeHistBlue();

	//计算直方图均值
	void computeAverage();

	//计算直方图方差
	void computeDeviation();

	//直方图均衡
	void histAve();
};
#endif // _INSIDE_VISUAL_CPP_ProcessBasedPixel