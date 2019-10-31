#pragma once
#include <vfw.h>

#ifndef AVIHandler_H
#define AVIHandler_H

#include "MediaSource.h"


class AVIHandler : MediaSource
{
public:
	AVIHandler(void);
	AVIHandler(CString filename);

public:
	~AVIHandler(void);
public:
	BOOL LoadVideo(CString filename);
private:
	int width; //图像宽度
	int height;//图象高度
	int length;//长度

	int defaultPad;

	BOOL loaded;

	CString AVIfilename; //AVI文件名

	LPBITMAPINFOHEADER lpbm;

	//未填充灰度缓冲（24Bit可彩色处理）
	unsigned char* gray_value;

	//边填充灰度缓冲（24Bit可彩色处理）
	unsigned char* gray_value_square;

	PGETFRAME pgf;

public:
	unsigned char* GetSingleFrame(int i);
	int GetFrameCount(void);

	int GetFrameWidth(void);

	int GetFrameHeight(void);

	LPBITMAPINFOHEADER GetLPBM(void);

	BOOL isLoaded(void);
};

#endif