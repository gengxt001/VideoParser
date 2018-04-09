#pragma once


class VideoFormat
{
private:
	CString m_strFilePath;
public:

	VideoFormat(CString strFilePath):m_strFilePath(strFilePath)
	{
		
	}

	virtual ~VideoFormat(void)
	{
	}

	CString getBasicFormatInfo();

};

