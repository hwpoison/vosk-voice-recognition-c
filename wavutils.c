#include "wavutils.h"

void playBuffer(char *rawdata, int nlen){
    play(rawdata, nlen);
}

void recordAndSaveWav(char *filename, int secs) {
    printf("[+] Recording %d seconds...\n", secs);
    int size = (secs * 2) * wav.sampleRate * 2;
    char *data = malloc(sizeof(char) * size);
    record(data, size);
    printf("[+] Saving .wav...\n");
    generateWav(filename, data, size);
    printf("[+] Recording finished.\n");
}

void generateWav(char *filename, char *rawdata, int size) { 
    FILE *fp;
    fp = fopen(filename, "wb");
    fwrite("RIFF", 4, 1, fp);                                   // 0-3 RIFF
    fwrite(&size, 4, 1, fp);                                    // 4-8 size
    fwrite("WAVE", 4, 1, fp);                                   // 9-11 WAVE
    fwrite("fmt ", 4, 1, fp);                                   // 12-14 fmt
    fwrite(to_little_endian(16), 4, 1, fp);                     // 15-18 Length of format data as listed above
    fwrite(to_little_endian(wav.formatTag), 2, 1, fp);          // 19-20 pFormat.wformatTag
    fwrite(to_little_endian(wav.nChannels), 2, 1, fp);          // 21-22 pFormat.nChannels
    fwrite(to_little_endian(wav.sampleRate), 4, 1, fp);         // 23-26 pFormat.nSamplesPerSec
    fwrite(to_little_endian(wav.avgBytesPerSecond), 4, 1, fp);  // 27-30 pFormat.nAvgBytesPerSec
    fwrite(to_little_endian(wav.blockAlign), 2, 1, fp);         // 31-32 pFormat.nBlockAlign
    fwrite(to_little_endian(wav.bitsPerSample), 2, 1, fp);      // 33-35 pFormat.wBitsPerSample
    fwrite("data", 4, 1, fp);                                   // 36-39 data indicator
    fwrite(&size, 4, 1, fp);                                    // 40-44 size of data
    fwrite(rawdata, size, 1, fp);                               // 45- until EOF
    fclose(fp);
}

uint32_t *to_little_endian(uint32_t x) {
    char *bytes = (char *) &x;
    uint32_t *y = (uint32_t *) malloc(sizeof(uint32_t));
    *y = (bytes[0] & 0xff) 
      | ((bytes[1] & 0xff) << 8) 
      | ((bytes[2] & 0xff) << 16) 
      | ((bytes[3] & 0xff) << 24);
    return y;
}

// return wav data length
long int getWavSize(FILE *wavin){
    fseek(wavin, 44, SEEK_END);
    return ftell(wavin);
}

void playWavFile(char *filename){
    printf("[+] Playing %s\n", filename);
    int nread, size;
    char *buffer;
    FILE *wavin;

    wavin = fopen(filename, "rb");

    // get sample rate
    fseek(wavin, 24, SEEK_SET);
    fread(&wav.sampleRate, 4, 1, wavin);

    // get bits per sample
    fseek(wavin, 34, SEEK_SET);
    fread(&wav.bitsPerSample, 2, 1, wavin);

    // get nchannels
    fseek(wavin, 22, SEEK_SET);
    fread(&wav.nChannels, 2, 1, wavin);

    // block align and avgBytespersecond
    wav.blockAlign = wav.nChannels * (wav.bitsPerSample / 8);
    wav.avgBytesPerSecond = wav.blockAlign * wav.sampleRate;

    // get total size
    size = getWavSize(wavin);
    buffer = malloc(sizeof(char) * size);

    // offset header
    fseek(wavin, 44, SEEK_SET); 
    while(!feof(wavin)){
        nread = fread(buffer, 1, size, wavin);
        playBuffer(buffer, nread);
    }

    fclose(wavin);
}