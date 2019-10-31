#pragma once
#include <winddi.h>

class MediaSource
{
public:
	virtual unsigned char* GetSingleFrame(int i) = 0;

	virtual int GetFrameCount(void) = 0;

	virtual int GetFrameWidth(void) = 0;

	virtual int GetFrameHeight(void) = 0;

	virtual LPBITMAPINFOHEADER GetLPBM(void) = 0;

	virtual BOOL isLoaded(void) = 0;

	virtual ~MediaSource() {
	}
};

