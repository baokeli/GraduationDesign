#pragma once

const static char key[16] = "PassWord:#1s$5f";
struct XYStruct
{
	enum XYID
	{
		XYID_CONNECT=1,
		XYID_SEND_VOICE,
	};
	unsigned int xyid;
	unsigned int msgLen;
	char msg[4096];
	XYStruct()
	{
		xyid = 0;
		msgLen = 0;
		memset(msg,0,sizeof(msg));
	};
	~XYStruct() {};
};