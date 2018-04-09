#pragma once
#include "videoformat.h"
class FlvFormat
{
public:
	FlvFormat(CString pFilePath);
	~FlvFormat(void);

	
	CString getBasicFormatInfo();

private:
	CString m_strFileName;
};

