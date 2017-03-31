#pragma once

struct XYStruct
{
	enum XYID
	{
		XYID_CONNECT,
		XYID_SEND_VOICE,
	};
	unsigned int xyid;
	unsigned int msgLen;
	char* msg;
	XYStruct()
	{
		xyid = 0;
		msgLen = 0;
		msg = 0;
	};
	~XYStruct() {};
};