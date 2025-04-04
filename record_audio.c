#include "record_audio.h"

HWAVEIN hWaveIn;
HWAVEOUT hWaveOut;

int init_input_device(int id){
    WAVEFORMATEX pFormat;
    pFormat.wFormatTag = wav.formatTag;     
    pFormat.nSamplesPerSec = wav.sampleRate;     
    pFormat.wBitsPerSample = wav.bitsPerSample;            
    pFormat.nChannels = wav.nChannels;
    pFormat.nBlockAlign = wav.blockAlign; 
    pFormat.nAvgBytesPerSec = wav.avgBytesPerSecond; 
    pFormat.cbSize = 0;
    
    if(waveInOpen(&hWaveIn, id, &pFormat,0L, 0L, WAVE_FORMAT_DIRECT)) return 1;


    atexit(close_audio);
}

int init_output_device(int id){
    WAVEFORMATEX pFormat;
    pFormat.wFormatTag = wav.formatTag;     
    pFormat.nSamplesPerSec = wav.sampleRate;     
    pFormat.wBitsPerSample = wav.bitsPerSample;            
    pFormat.nChannels = wav.nChannels;
    pFormat.nBlockAlign = wav.blockAlign; 
    pFormat.nAvgBytesPerSec = wav.avgBytesPerSecond; 
    pFormat.cbSize = 0;
    
    if(waveOutOpen(&hWaveOut, id, &pFormat,0L, 0L, WAVE_FORMAT_DIRECT)) return 1;
    atexit(close_audio);
}

//Length is typically 44100*time...
int record(char * data, int length)
{
    if(hWaveIn == NULL){
        init_input_device(WAVE_MAPPER);
    }
    WAVEHDR WaveInHdr;

    WaveInHdr.lpData = (LPSTR)data;
    WaveInHdr.dwBufferLength = length;
    WaveInHdr.dwBytesRecorded=0;
    WaveInHdr.dwUser = 0L;
    WaveInHdr.dwFlags = 0L;
    WaveInHdr.dwLoops = 0L;
    waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

    if(waveInStart(hWaveIn)) return 3;

    if(waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR))) return 2;
    while(waveInUnprepareHeader(hWaveIn,&WaveInHdr,sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) Sleep(1);

    return 0;
}
 
int play(char *data, int length)
{
    if(hWaveIn == NULL){
        init_output_device(WAVE_MAPPER);
    }
    WAVEHDR WaveOutHdr;

    WaveOutHdr.lpData = (LPSTR)data;
    WaveOutHdr.dwBufferLength = length;
    WaveOutHdr.dwBytesRecorded=0;
    WaveOutHdr.dwUser = 0L;
    WaveOutHdr.dwFlags = 0L;
    WaveOutHdr.dwLoops = 0L;
    waveOutPrepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
    
    if(waveOutWrite(hWaveOut,&WaveOutHdr,sizeof(WAVEHDR))) return 2;
    while(waveOutUnprepareHeader(hWaveOut,&WaveOutHdr,sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) Sleep(1);

    return 0;
}

void close_audio() {
    if (hWaveIn) {
        waveInClose(hWaveIn);
        hWaveIn = NULL;
    }
    if (hWaveOut) {
        waveOutClose(hWaveOut);
        hWaveOut = NULL;
    }
}


void list_input_devices() {
    UINT nDevices = waveInGetNumDevs();
    WAVEINCAPS caps;

    printf("Audio input devices:\n");
    for (UINT i = 0; i < nDevices; i++) {
        if (waveInGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
            printf("  ID %u: %s\n", i, caps.szPname);
        }
    }
}

void list_output_devices() {
    UINT nDevices = waveOutGetNumDevs();
    WAVEOUTCAPS caps;

    printf("Audio output devices:\n");
    for (UINT i = 0; i < nDevices; i++) {
        if (waveOutGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
            printf("  ID %u: %s\n", i, caps.szPname);
        }
    }
}