#ifndef FLV_FORMAT
#define FLV_FORMAT

#pragma once
#include <list>
#include "videoformat.h"
#include <map>

using namespace std;

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct
{
	//byte Signature[3];
	CString strType;
	uint begin_pos;
	uint end_pos;
	map<CString, CString> mapExtraInfo;
	list<CString> listExtraInfo;
}HEX_FORMAT;

class FlvFormat
{
public:
	FlvFormat(CString pFilePath);
	~FlvFormat(void);

	
	list<CString> getBasicFormatInfo();
	list<HEX_FORMAT> parseFile();
private:
	CString m_strFileName;

	int err_no;
};
#endif

