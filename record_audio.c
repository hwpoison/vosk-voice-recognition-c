#include "record_audio.h"

//Length is typically 44100*time...
int record(char * data, int length)
{
    HWAVEIN hWaveIn;
    WAVEHDR WaveInHdr;

    WAVEFORMATEX pFormat;
    pFormat.wFormatTag = wav.formatTag;     
    pFormat.nSamplesPerSec = wav.sampleRate;     
    pFormat.wBitsPerSample = wav.bitsPerSample;            
    pFormat.nChannels = wav.nChannels;
    pFormat.nBlockAlign = wav.blockAlign; 
    pFormat.nAvgBytesPerSec = wav.avgBytesPerSecond; 
    pFormat.cbSize = 0;
 
    if(waveInOpen(&hWaveIn, WAVE_MAPPER ,&pFormat,0L, 0L, WAVE_FORMAT_DIRECT)) return 1;
     
    WaveInHdr.lpData = (LPSTR)data;
    WaveInHdr.dwBufferLength = length;
    WaveInHdr.dwBytesRecorded=0;
    WaveInHdr.dwUser = 0L;
    WaveInHdr.dwFlags = 0L;
    WaveInHdr.dwLoops = 0L;
    waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
 
    if(waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR))) return 2;
 
    if(waveInStart(hWaveIn)) return 3;
    while(waveInUnprepareHeader(hWaveIn,&WaveInHdr,sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) Sleep(1);
    waveInClose(hWaveIn);
    return 0;
}
 
int play(char *data, int length)
{
    HWAVEOUT hWaveOut;
    WAVEHDR WaveOutHdr;
 
    WAVEFORMATEX pFormat;
    pFormat.wFormatTag = wav.formatTag;     
    pFormat.nSamplesPerSec = wav.sampleRate;     
    pFormat.wBitsPerSample = wav.bitsPerSample;            
    pFormat.nChannels = wav.nChannels;
    pFormat.nBlockAlign = wav.blockAlign; 
    pFormat.nAvgBytesPerSec = wav.avgBytesPerSecond; 
    pFormat.cbSize = 0;
    
    if(waveOutOpen(&hWaveOut, WAVE_MAPPER,&pFormat,0L, 0L, WAVE_FORMAT_DIRECT)) return 1;
 
    WaveOutHdr.lpData = (LPSTR)data;
    WaveOutHdr.dwBufferLength = length;
    WaveOutHdr.dwBytesRecorded=0;
    WaveOutHdr.dwUser = 0L;
    WaveOutHdr.dwFlags = 0L;
    WaveOutHdr.dwLoops = 0L;
    waveOutPrepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
    
    if(waveOutWrite(hWaveOut,&WaveOutHdr,sizeof(WAVEHDR))) return 2;
    while(waveOutUnprepareHeader(hWaveOut,&WaveOutHdr,sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) Sleep(1);
    waveOutClose(hWaveOut);
    return 0;
}
