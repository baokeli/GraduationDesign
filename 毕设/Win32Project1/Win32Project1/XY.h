#pragma once

struct XYStruct
{
	enum XYID
	{
		XYID_CONNECT=1,
		XYID_SEND_VOICE,
	};
	unsigned int xyid;
	unsigned int msgLen;
	char msg[1024];
	XYStruct()
	{
		xyid = 0;
		msgLen = 0;
		memset(msg,0,sizeof(msg));
	};
	~XYStruct() {};
};