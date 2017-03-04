#include "StdAfx.h"
#include "GWaveIn.h"

GWaveIn::GWaveIn(void)
{
}

GWaveIn::~GWaveIn(void)
{
}

bool GWaveIn::IfWaveIn()
{
	MMRESULT mmresult = 0;

	mmresult = waveInGetNumDevs();
	if(mmresult == 0) return false;

	WAVEINCAPS waveincaps = {0};
	unsigned int i = 0;
	unsigned int num = mmresult;
	for( ; i<num; i++)
	{
		mmresult = waveInGetDevCaps(i,&waveincaps,sizeof(WAVEINCAPS));
		if(mmresult != MMSYSERR_NOERROR) 
			return false;
		if((waveincaps.dwFormats & WAVE_FORMAT_1M08) == 0) 
			continue;
		else 
		{
			m_iWaveInID = i;
			return true;
		}
	}

	return false;
}

bool GWaveIn::PrepareWaveIn(HWND hwnd, BYTE* pbuf1, BYTE* pbuf2)
{
	WAVEFORMATEX waveformatex = {0};
	waveformatex.wFormatTag = WAVE_FORMAT_PCM;
	waveformatex.nChannels = 1;
	waveformatex.nSamplesPerSec = 11025;
	waveformatex.nAvgBytesPerSec = 11025*1*8/8;
	waveformatex.nBlockAlign = 8*1/8;
	waveformatex.wBitsPerSample = 8;
	waveformatex.cbSize = 0;

	MMRESULT mmresult = 0;
	mmresult = waveInOpen(&m_hWaveIn,m_iWaveInID,&waveformatex,(DWORD)hwnd,0,CALLBACK_WINDOW);
	if(mmresult != MMSYSERR_NOERROR) return false;

	
	m_head1.lpData = (LPSTR)pbuf1;
	m_head1.dwBufferLength = 20480;
	m_head1.dwBytesRecorded = 0;
	m_head1.dwUser = 0;
	m_head1.dwFlags = 0;
	m_head1.dwLoops = 1;
	m_head1.lpNext = NULL;
	m_head1.reserved = 0;

	mmresult = waveInPrepareHeader(m_hWaveIn,&m_head1,sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR) return false;

	m_head2.lpData = (LPSTR)pbuf2;
	m_head2.dwBufferLength = 20480;
	m_head2.dwBytesRecorded = 0;
	m_head2.dwUser = 0;
	m_head2.dwFlags = 0;
	m_head2.dwLoops = 1;
	m_head2.lpNext = NULL;
	m_head2.reserved = 0;

	mmresult = waveInPrepareHeader(m_hWaveIn,&m_head2,sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR) return false;

	return true;
}

bool GWaveIn::AddBuffer(PWAVEHDR pwavehdr)
{
	MMRESULT mmresult = 0;
	mmresult = waveInAddBuffer(m_hWaveIn,pwavehdr,sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR) return false;

	return true;
}

bool GWaveIn::StartRec()
{
	MMRESULT mmresult = 0;
	mmresult = waveInStart(m_hWaveIn);
	if(mmresult != MMSYSERR_NOERROR) return false;

	return true;
}

bool GWaveIn::CloseWaveIn()
{
	MMRESULT mmresult = 0;
	waveInStop(m_hWaveIn);
	waveInReset(m_hWaveIn);
	waveInClose(m_hWaveIn);

	if(m_head1.lpData != NULL) free(m_head1.lpData);
	waveInUnprepareHeader(m_hWaveIn,&m_head1,sizeof(WAVEHDR));

	if(m_head2.lpData != NULL) free(m_head2.lpData);
	waveInUnprepareHeader(m_hWaveIn,&m_head2,sizeof(WAVEHDR));

	return true;
}

