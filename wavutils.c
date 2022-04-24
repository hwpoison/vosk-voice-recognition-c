#include "wavutils.h"

uint32_t *to_little_endian(uint32_t x) {
    char *bytes = (char *) &x;
    uint32_t *y = (uint32_t *) malloc(sizeof(uint32_t));
    *y = (bytes[0] & 0xff) 
      | ((bytes[1] & 0xff) << 8) 
      | ((bytes[2] & 0xff) << 16) 
      | ((bytes[3] & 0xff) << 24);
    return y;
}

void playAudioBuffer(char *rawdata, int nlen){
    startPlayback(rawdata, nlen);
}

void recordAndSaveWav(char *filename, int secs) {
    printf("[+] Recording %d seconds...\n", secs);
    int size = (secs * SAMPLERATE * 2);
    char *data = malloc(sizeof(char) * size);
    startRecord(data, size);
    printf("[+] Saving .wav...\n");
    generateWav(filename, data, size);
    printf("[+] Recording finished.\n");
}

void generateWav(char *filename, char *rawdata, int size) { 
    FILE *fp;
    fp = fopen(filename, "wb");
    fwrite("RIFF", 4, 1, fp);                               // 0-3 RIFF
    fwrite(&size, 4, 1, fp);                                // 4-8 size
    fwrite("WAVE", 4, 1, fp);                               // 9-11 WAVE
    fwrite("fmt ", 4, 1, fp);                               // 12-14 fmt
    fwrite(to_little_endian(16), 4, 1, fp);                 // 15-18 Length of format data as listed above
    fwrite(to_little_endian(FORMAT_TAG), 2, 1, fp);         // 19-20 pFormat.wformatTag
    fwrite(to_little_endian(NCHANNELS), 2, 1, fp);          // 21-22 pFormat.nChannels
    fwrite(to_little_endian(SAMPLERATE), 4, 1, fp);         // 23-26 pFormat.nSamplesPerSec
    fwrite(to_little_endian(AVG_BYTE_PER_SECOND), 4, 1, fp);// 27-30 pFormat.nAvgBytesPerSec
    fwrite(to_little_endian(BLOCK_ALIGN), 2, 1, fp);        // 31-32 pFormat.nBlockAlign
    fwrite(to_little_endian(BITSPERSAMPLE), 2, 1, fp);      // 33-35 pFormat.wBitsPerSample
    fwrite("data", 4, 1, fp);                               // 36-39 data indicator
    fwrite(&size, 4, 1, fp);                                // 40-44 size of data
    fwrite(rawdata, size, 1, fp);                              // 45-size
    fclose(fp);
}
// return wav data length
long int GetWavSize(FILE *wavin){
    fseek(wavin, 44, SEEK_END);
    return ftell(wavin);
}

void playWavFile(char *filename){
    printf("[+] Playing %s\n", filename);
    int nread, size;
    char *buffer;
    FILE *wavin;

    wavin = fopen(filename, "rb");
    size = GetWavSize(wavin);
    buffer = malloc(sizeof(char) * size);

    fseek(wavin, 44, SEEK_SET); // offset header
    while(!feof(wavin)){
        nread = fread(buffer, 1, size, wavin);
        playAudioBuffer(buffer, nread);
    }

    fclose(wavin);
}



