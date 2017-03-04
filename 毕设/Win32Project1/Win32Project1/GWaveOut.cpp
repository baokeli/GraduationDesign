#include "StdAfx.h"
#include "GWaveOut.h"

GWaveOut::GWaveOut(void)
{
}

GWaveOut::~GWaveOut(void)
{
}

bool GWaveOut::IfWaveOut()
{
	MMRESULT mmresult = 0;
    mmresult = waveOutGetNumDevs();
	if(mmresult == 0) return false;

	WAVEOUTCAPS waveoutcaps = {0};
	unsigned int i = 0;
	unsigned int num = mmresult;
	for( ; i<num; i++)
	{
		mmresult = waveOutGetDevCaps(i,&waveoutcaps,sizeof(WAVEOUTCAPS));
		if(mmresult != MMSYSERR_NOERROR) return false;
		if((waveoutcaps.dwFormats & WAVE_FORMAT_1M08) == 0)
			continue;
		else
		{
			m_iWaveOutID = i;
			return true;
		}
	}

	return false;
}

bool GWaveOut::PrepareWaveOut(HWND hwnd, BYTE* pbuf1, BYTE* pbuf2)
{
	WAVEFORMATEX waveformatex = {0};
	waveformatex.wFormatTag = WAVE_FORMAT_PCM;
	waveformatex.nChannels = 1;
	waveformatex.cbSize = 0;
	waveformatex.nAvgBytesPerSec = 11025*1*8/8;
	waveformatex.nBlockAlign = 1;
	waveformatex.nSamplesPerSec = 11025;
	waveformatex.wBitsPerSample = 8;

	MMRESULT mmresult = 0;
	mmresult = waveOutOpen(&m_hWaveOut,m_iWaveOutID,&waveformatex,(DWORD)hwnd,0,CALLBACK_WINDOW);
	if(mmresult != MMSYSERR_NOERROR) return false;

	m_head1.lpData = (LPSTR)pbuf1;
	m_head1.dwBufferLength = 20480;
	m_head1.dwBytesRecorded = 0;
	m_head1.dwUser = 0;
	m_head1.dwFlags = 0;
	m_head1.dwLoops = 1;
	m_head1.lpNext = NULL;
	m_head1.reserved = 0;

	mmresult = waveOutPrepareHeader(m_hWaveOut,&m_head1,sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR) return false;

	m_head2.lpData = (LPSTR)pbuf2;
	m_head2.dwBufferLength = 20480;
	m_head2.dwBytesRecorded = 0;
	m_head2.dwUser = 0;
	m_head2.dwFlags = 0;
	m_head2.dwLoops = 1;
	m_head2.lpNext = NULL;
	m_head2.reserved = 0;

	mmresult = waveOutPrepareHeader(m_hWaveOut,&m_head2,sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR) return false;

	return true;
}

bool GWaveOut::WaveOutWrite(PWAVEHDR pwavehdr)
{
	MMRESULT mmresult = 0;
	waveOutWrite(m_hWaveOut,pwavehdr,sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR) return false;

	return true;
}

bool GWaveOut::CloseWaveOut()
{
	waveOutReset(m_hWaveOut);//停止声音处理并产生MM_WOM_DONE消息，关键
	waveOutClose(m_hWaveOut);

	if(m_head1.lpData != NULL) free(m_head1.lpData);
	waveOutUnprepareHeader(m_hWaveOut,&m_head1,sizeof(WAVEHDR));

	if(m_head2.lpData != NULL) free(m_head2.lpData);
	waveOutUnprepareHeader(m_hWaveOut,&m_head2,sizeof(WAVEHDR));

	return true;
}