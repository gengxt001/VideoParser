#include "StdAfx.h"
#include "FlvFormat.h"
#include <iostream>
#include <AtlBase.h>

using namespace std;

#pragma pack(1)

//************ Constants
#define TAG_TYPE_AUDIO 0x08
#define TAG_TYPE_VIDEO 0x09
#define TAG_TYPE_SCRIPT 0x12

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

list<CString> audio_type_parse(char tagdata_first_byte)
{
	list<CString> listStrRtn;
	CString strInfo;
	
	int x=tagdata_first_byte&0xF0;  
	x=x>>4;  
	CString strAudioEncodeType;
	switch (x)  
	{  
		case 0:		strAudioEncodeType = "Linear PCM, platform endian";break;  
		case 1:		strAudioEncodeType = "ADPCM";break;  
		case 2:		strAudioEncodeType = "MP3";break;  
		case 3:		strAudioEncodeType = "Linear PCM, little endian";break;  
		case 4:		strAudioEncodeType = "Nellymoser 16-kHz mono";break;  
		case 5:		strAudioEncodeType = "Nellymoser 8-kHz mono";break;  
		case 6:		strAudioEncodeType = "Nellymoser";break;  
		case 7:		strAudioEncodeType = "G.711 A-law logarithmic PCM";break;  
		case 8:		strAudioEncodeType = "G.711 mu-law logarithmic PCM";break;  
		case 9:		strAudioEncodeType = "reserved";break;  
		case 10:	strAudioEncodeType = "AAC";break;  
		case 11:	strAudioEncodeType = "Speex";break;  
		case 14:	strAudioEncodeType = "MP3 8-Khz";break;  
		case 15:	strAudioEncodeType = "Device-specific sound";break;  
		default:	strAudioEncodeType = "UNKNOWN";break;  
	}  
	

	x=tagdata_first_byte&0x0C;  
	x=x>>2;  
	CString strAudioSampleRate;
	switch (x)  
	{  
	case 0: strAudioSampleRate = "5.5-kHz";break;  
	case 1: strAudioSampleRate = "1-kHz";break;  
	case 2: strAudioSampleRate = "22-kHz";break;  
	case 3: strAudioSampleRate = "44-kHz";break;  
	default: strAudioSampleRate = "UNKNOWN";break;  
	}  
	
	x=tagdata_first_byte&0x02;  
	x=x>>1;  
	CString strAudioPrecisioin;
	switch (x)  
	{  
	case 0:	strAudioPrecisioin ="8Bit";break;  
	case 1:	strAudioPrecisioin ="16Bit";break;  
	default:strAudioPrecisioin ="UNKNOWN";break;  
	}  
	
	x=tagdata_first_byte&0x01;  
	CString strAudioType;
	switch (x)  
	{  
	case 0:	strAudioType ="Mono";break;  
	case 1:	strAudioType ="Stereo";break;  
	default:strAudioType ="UNKNOWN";break;  
	}  
	
	strInfo.Format(_T("Audio EncodeType: %s"), strAudioEncodeType);
	listStrRtn.push_back(strInfo);
	strInfo.Format(_T("Audio SampleRate: %s"), strAudioSampleRate);
	listStrRtn.push_back(strInfo);
	strInfo.Format(_T("Audio Precisioin: %s"), strAudioPrecisioin);
	listStrRtn.push_back(strInfo);
	strInfo.Format(_T("Audio Type: %s"), strAudioType);
	listStrRtn.push_back(strInfo);

	return listStrRtn;	
}

list<CString> video_type_parse(char tagdata_first_byte)
{
	list<CString> listStrRtn;
	CString strInfo;

	int x=tagdata_first_byte&0xF0;  
    x=x>>4; 
	CString strVideoFrameType;
    switch (x)  
    {  
    case 1:	strVideoFrameType = "key frame  ";break;  
    case 2:	strVideoFrameType = "inter frame";break;  
    case 3:	strVideoFrameType = "disposable inter frame";break;  
    case 4:	strVideoFrameType = "generated keyframe";break;  
    case 5:	strVideoFrameType = "video info/command frame";break;  
    default:strVideoFrameType = "UNKNOWN";break;  
    }  
	strInfo.Format(_T("Video FrameType: %s"), strVideoFrameType);
	listStrRtn.push_back(strInfo);

    CString strVideoEncodeType;
	x=tagdata_first_byte&0x0F;  
    switch (x)  
    {  
    case 1:	strVideoEncodeType = "JPEG (currently unused)";break;  
    case 2:	strVideoEncodeType = "Sorenson H.263";break;  
    case 3:	strVideoEncodeType = "Screen video";break;  
    case 4:	strVideoEncodeType = "On2 VP6";break;  
    case 5:	strVideoEncodeType = "On2 VP6 with alpha channel";break;  
    case 6:	strVideoEncodeType = "Screen video version 2";break;  
    case 7:	strVideoEncodeType = "AVC";break;  
    default:strVideoEncodeType = "UNKNOWN";break;  
    } 
	strInfo.Format(_T("Video EncodeType: %s"), strVideoEncodeType);
	listStrRtn.push_back(strInfo);

	return listStrRtn;	
}

list<CString> FlvFormat::getBasicFormatInfo()
{
    list<CString> listStrRtn;
	//strRtn += "222";

	int nSize = sizeof(FLV_HEADER);
	
	FLV_HEADER flvHeader;

    FILE *pFile = _tfopen(m_strFileName,_T("rb"));

	CString Signature;
	CString version;
	CString flags;
	CString dataoffset;
    if (pFile != NULL)
	{		
	
		int ret = fread((char*)&flvHeader, 1, nSize, pFile);

		Signature.Format(_T("Signature:  0x %c %c %c\n"),flvHeader.Signature[0],flvHeader.Signature[1],flvHeader.Signature[2]);
		version.Format(_T("Version:    0x %X\n"),flvHeader.Version);
		flags.Format(_T("Flags  :    0x %X\n") ,flvHeader.Flags);
		dataoffset.Format(_T("HeaderSize: 0x %X\n"),reverse_bytes((byte *)&flvHeader.DataOffset, sizeof(flvHeader.DataOffset))); 
	}

	listStrRtn.push_back(Signature);
	listStrRtn.push_back(version);
	listStrRtn.push_back(flags);
	listStrRtn.push_back(dataoffset);

	//strRtn = Signature + version + flags + dataoffset;

	fclose(pFile);



	return listStrRtn;
}

list<HEX_FORMAT> FlvFormat::parseFile()
{
	FLV_HEADER flvHeader;
	TAG_HEADER tagHeader;
	
	list<HEX_FORMAT> listFormat;

	int nFlvHeaderSize = sizeof(FLV_HEADER);

    FILE *pFile = _tfopen(m_strFileName,_T("rb"));
	if (pFile== NULL)
	{
		err_no = 1;
		return listFormat;
	}

	int ret = fread(&flvHeader, 1, nFlvHeaderSize, pFile);

	fseek(pFile, reverse_bytes((byte *)&flvHeader.DataOffset, sizeof(flvHeader.DataOffset)), SEEK_SET);
	
	uint previoustagsize, previoustagsize_z=0;
	uint ts=0, ts_new=0, ts_offset=0, ptag=0;

	HEX_FORMAT hex_Format;

	hex_Format.begin_pos = 0;
	hex_Format.end_pos = sizeof(FLV_HEADER)-1;//这里的减1是因为计算机的起始位置从0开始
	hex_Format.strType = "Flv Header";
	//下面分析FlvHeader
	CString strSignature;
	CString version;
	CString flags;
	CString dataoffset;
	strSignature.Format(_T("Signature:  0x %c %c %c"),flvHeader.Signature[0],flvHeader.Signature[1],flvHeader.Signature[2]);
	version.Format(_T("Version:    0x %X"),flvHeader.Version);
	flags.Format(_T("Flags  :    0x %X") ,flvHeader.Flags);
	dataoffset.Format(_T("HeaderSize: 0x %X"),reverse_bytes((byte *)&flvHeader.DataOffset, sizeof(flvHeader.DataOffset))); 
	hex_Format.listExtraInfo.push_back(strSignature);

	listFormat.push_back(hex_Format);
	
	int begin_pos = hex_Format.end_pos+1;

	do
	{
		previoustagsize = _getw(pFile);

		hex_Format.begin_pos = hex_Format.end_pos + 1; //这里的加1，是因为下一个的begin_pos 是在上一个的end_pos基础上后移一个
		hex_Format.end_pos = hex_Format.begin_pos +4 -1; //这里的加4，是因为长度为4， 减1同上面减1的理由
		hex_Format.strType = "Previous Tag Size";
		//下面分析PreviousTagSize
		CString strPreviousTagSize;
		strPreviousTagSize.Format(_T("PreviousTagSize=%d"), previoustagsize);
		hex_Format.listExtraInfo.push_back(strPreviousTagSize);

		listFormat.push_back(hex_Format);


		fread((void*)&tagHeader, sizeof(TAG_HEADER), 1, pFile);

		int temp_datasize = tagHeader.DataSize[0]*65536 + tagHeader.DataSize[1]*256 + tagHeader.DataSize[2];
		int temp_timestamp = tagHeader.TimeStamp[0]*65536 + tagHeader.TimeStamp[1]*256 + tagHeader.TimeStamp[2];

		ptag = tagHeader.TagType;
		if(feof(pFile))
		{
			break;
		}

		//begin_pos = 4;
		hex_Format.begin_pos =  hex_Format.end_pos + 1;
		hex_Format.end_pos = hex_Format.begin_pos +sizeof(TAG_HEADER) + temp_datasize -1;

		//以下的每个case中，都会先读取第一个字节，这第一个字节指的是TagHeader后面中TagData的第一个字节
		//该字节针对音频、视频、ScriptData 有不同的含义
		switch (ptag)
		{
		case TAG_TYPE_AUDIO:
			{
				char tagdata_first_byte=fgetc(pFile);
				//这里可以分析Video第一个字节
				list<CString> listAudioInfo = audio_type_parse(tagdata_first_byte);
				//.....分析 第一个字节
				int data_size=reverse_bytes((byte *)&tagHeader.DataSize, sizeof(tagHeader.DataSize))-1;
				for (int i=0; i<data_size; i++)
						fgetc(pFile);

				hex_Format.strType = "AudioTag";
				hex_Format.listExtraInfo = listAudioInfo;

				break;
			}
		case TAG_TYPE_VIDEO:
			{
				char tagdata_first_byte=fgetc(pFile);
				//这里可以分析Video第一个字节
				list<CString> listVideoInfo = video_type_parse(tagdata_first_byte);
				//.....分析 第一个字节

				//倒回去，不影响下面的操作
				fseek(pFile, -1, SEEK_CUR);
				//这里连带着读取了下面一个的previousTagSize
				int data_size=reverse_bytes((byte *)&tagHeader.DataSize, sizeof(tagHeader.DataSize))+4;
				for (int i=0; i<data_size; i++)
						fgetc(pFile);
				fseek(pFile, -4, SEEK_CUR);//这里又倒回去

				hex_Format.strType = "VideoTag";
				hex_Format.listExtraInfo = listVideoInfo;
				break;
			}
		case TAG_TYPE_SCRIPT:
			{
				char tagdata_first_byte=fgetc(pFile); //读取tagdata的第一个字节
				//这里可以分析Script第一个字节
				//.....分析 第一个字节

				//这里返回到TagData之前
				fseek(pFile, -1, SEEK_CUR);
				//这里从TagData的开始往后移
				fseek(pFile, reverse_bytes((byte *)&tagHeader.DataSize, sizeof(tagHeader.DataSize)), SEEK_CUR);
				
				//改为读取TagData，然后分析

				hex_Format.strType = "ScriptTag";
				break;
			}
		default:
			{
				fseek(pFile, reverse_bytes((byte *)&tagHeader.DataSize, sizeof(tagHeader.DataSize)), SEEK_CUR);
				
				hex_Format.strType = "NULLTag";

				break;
			}
		}
		
		listFormat.push_back(hex_Format);
		
		begin_pos = hex_Format.end_pos + 1;


	}while(!feof(pFile));

	return listFormat;
}
