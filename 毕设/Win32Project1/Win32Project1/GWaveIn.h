#pragma once

#include <Windows.h>
#include <MMSystem.h>
using namespace std;

class GWaveIn
{
public:
	GWaveIn(void);
	virtual ~GWaveIn(void);

	bool IfWaveIn();                //检查设备
	bool PrepareWaveIn(HWND hwnd, BYTE* pbuf1, BYTE* pbuf2);           //录音的准备工作
	bool AddBuffer(PWAVEHDR pwavehdr);
	bool StartRec();
	bool CloseWaveIn();

public:
	HWAVEIN    m_hWaveIn;
	UINT_PTR   m_iWaveInID;
	WAVEHDR   m_head1;
	WAVEHDR   m_head2;
};
