#include "StdAfx.h"
#include "FlvFormat.h"
#include <iostream>
#include<AtlBase.h>

using namespace std;

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct
{
	byte Signature[3];
	byte Version;
	byte Flags;
	uint DataOffset;
}FLV_HEADER;

typedef struct
{
	byte TagType;
	byte DataSize[3];
	byte TimeStamp[3];
	uint Reserved;
}TAG_HEADER;
//reverse_bytes - turn a BigEndian byte array into a LittleEndian integer  
uint reverse_bytes(byte *p, char c) {  
    int r = 0;  
    int i;  
    for (i=0; i<c; i++)   
        r |= ( *(p+i) << (((c-1)*8)-8*i));  
    return r;  
}  

FlvFormat::FlvFormat(CString strFilePath)
{
	m_strFileName = strFilePath;
	std::cout<<"filePath = "<<std::endl;
}


FlvFormat::~FlvFormat(void)
{
}

CString FlvFormat::getBasicFormatInfo()
{
    CString strRtn;
	strRtn += "222";

	int nSize = sizeof(FLV_HEADER);
	
	FLV_HEADER flvHeader;

    FILE *pFile = _tfopen(m_strFileName,_T("rb"));

	CString Signature;
	CString version;
	CString flags;
	CString dataoffset;
    if (pFile != NULL)
	{		
		int ret = fread(&flvHeader, 1, nSize, pFile);
		Signature.Format(_T("Signature:  0x %c %c %c\n"),flvHeader.Signature[0],flvHeader.Signature[1],flvHeader.Signature[2]);
		version.Format(_T("Version:    0x %X\n"),flvHeader.Version);
		flags.Format(_T("Flags  :    0x %X\n") ,flvHeader.Flags);
		dataoffset.Format(_T("HeaderSize: 0x %X\n"),reverse_bytes((byte *)&flvHeader.DataOffset, sizeof(flvHeader.DataOffset))); 
	}

	strRtn = Signature + version + flags + dataoffset;

	fclose(pFile);



	return strRtn;
}

