#pragma once

#include <windows.h>
#include <MMSystem.h>
using namespace std;

class GWaveOut
{
public:
	GWaveOut(void);
	virtual ~GWaveOut(void);

	bool IfWaveOut();
	bool PrepareWaveOut(HWND hwnd, BYTE* pbuf1, BYTE* pbuf2);
	bool WaveOutWrite(PWAVEHDR pwavehdr);
	bool CloseWaveOut();

public:
	HWAVEOUT m_hWaveOut;
	UINT_PTR m_iWaveOutID;
	WAVEHDR m_head1;
	WAVEHDR m_head2;
};
