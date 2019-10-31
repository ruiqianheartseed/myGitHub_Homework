#ifndef _INSIDE_VISUAL_CPP_GRAYTRANS
#define _INSIDE_VISUAL_CPP_GRAYTRANS
#include "ImageCenterDib.h"

//灰度变换类
class GrayTrans :public ImgCenterDib
{
public:
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
	GrayTrans();

	//带参数的构造函数
	GrayTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		unsigned char *pImgData);

	//析构函数
	~GrayTrans();

	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();

	//负相
	void ReverseImg();

	//彩色变灰度格式
	void ColorToGray();

	//灰度变彩色格式
	void GrayToColor();

	//二值化
	void Binary(int threshold = 128);
	void Binaryforline(int threshold = 128);
	void Clearlineave(int threshold = 128);
	void Clearlinemid(int threshold = 128);
	//线性拉伸
	void LinearStrech(CPoint point[2]);

	//对数非线性拉伸
	void DuiShuStrech(float a, float b, float c);

	//指数非线性拉伸
	void ZhiShuStrech(float a, float b, float c);

private:
	//单通道数据线性拉伸
	void LinearStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut, int imgWidth, int imgHeight, CPoint point[2]);

	//单通道数据对数函数非线性拉伸
	void DuiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut, int imgWidth, int imgHeight,
		float a, float b, float c);

	//单通道数据指数函数非线性拉伸
	void ZhiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut, int imgWidth, int imgHeight,
		float a, float b, float c);

};
#endif // _INSIDE_VISUAL_CPP_GRAYTRANS