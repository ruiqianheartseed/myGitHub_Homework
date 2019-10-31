#ifndef _INSIDE_VISUAL_CPP_IMGSEGMENT
#define _INSIDE_VISUAL_CPP_IMGSEGMENT
#include "ImageCenterDib.h"

//图像分割类
class ImgSegment:public ImgCenterDib
{
public:
	//输出图像每像素位数
	int m_nBitCountOut;

	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;

	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;
private:
	//输出图像的宽
	int m_imgWidthOut;

	//输出图像的高
	int m_imgHeightOut;

	//输出图像颜色表长度
	int m_nColorTableLengthOut;
public:
	//不带参数的构造函数
	ImgSegment();

	//带参数的构造函数
	ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		        unsigned char *pImgData);

	//析构函数
	~ImgSegment();

	//以像素为单位返回输出图像的尺寸
	CSize GetDimensions();

	
	int calculate(int histArray[256]);

	//大津阈值分割
	int threshOtus(int histArray[256]);

	//Roberts算子
	void Roberts();

	//Sobel算子
	void Sobel();

	//Prewitt算子
	void Prewitt();

	//Laplacian算子
	void Laplacian();

	//Krisch算子
	void Krisch();

	//Gauss-Laplacian算子
	void GaussLaplacian();

	//自定义模板检测边缘
	void EdgeByAnyMask(int *mask, int maskW, int maskH);

	//Hough变换
	void Hough(float radiusResolution, float angleResolution,
		float *radius, float *angle);

	//通过Hough变换检测图像中最长线
	void longestLineDetectByHough(float radiusResolution, float angleResolution);

	//区域生长
	void RegionGrow(CPoint seed, int thresh);

	//轮廓提取
	void ContourExtract();

	//曲线跟踪
	void ContourTrace();
	







private:
	//模板卷积函数
	void TemplateEdge(unsigned char *imgIn, int width, int height, int nBitCount,
		int *mask, int maskW, int maskH, unsigned char *imgOut);
	//极坐标形式下的线检测hough变换
	void HoughTransform(unsigned char *imgBinaryIn, int width, int height, int *houghBuf,
		int  houghWidth, int houghHeight,float radiusResolution, float angleResolution,
					 float *radius, float *angle);
};
#endif // _INSIDE_VISUAL_CPP_IMGSEGMENT