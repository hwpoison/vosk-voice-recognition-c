#include "recognition.h"
#include "record_audio.c"

#define true 1

void enqueue_audio_block(struct audio_block *data) {
    if (queue_size == 0) {
        queue_head = data;
        queue_tail = data;
    } else {
        queue_tail->next = data;
        queue_tail = data;
    }
    queue_size++;
}

struct audio_block *dequeue_audio_block() {
    if (queue_size == 0) {
        return NULL;
    }
    struct audio_block *data = queue_head;
    queue_head = queue_head->next;
    queue_size--;
    return data;
}

void *getBlockFromMic(){
    printf("[+] Running recording thread for getBlockFromMic ...\n");
    while(true){
        struct audio_block *block = malloc(sizeof(struct audio_block));
        block->size = BLOCK_SIZE;
        block->data = malloc(sizeof(char) * block->size);
        startRecord(block->data, block->size);
        enqueue_audio_block(block);
       // Sleep(1);
    }
}

void *getBlockFromFile(void *filename){
    printf("[+] Running recording thread for getBlockFromFile ...\n");
    FILE *wavin;
    char buffer[BLOCK_SIZE];
    int nread;
    wavin = fopen((char*)filename, "rb");
    if (wavin == NULL) {
        printf("[x] Error opening file\n");
    }else{
        fseek(wavin, 44, SEEK_SET); // offset 44 bytes of wav header
        while(!feof(wavin)){
            struct audio_block *block = malloc(sizeof(struct audio_block));
            nread = fread(buffer, 1, sizeof(buffer), wavin);
            block->size = nread;
            block->data = malloc(nread);
            memcpy(block->data, buffer, nread);
            enqueue_audio_block(block);
        } 
        fclose(wavin);
    }
}

void recognizeAudioBlock(VoskRecognizer *recognizer, char *data, int nlen){
    int final = vosk_recognizer_accept_waveform(recognizer, data, nlen);
    if (final) {
     printf("%s\n", vosk_recognizer_result(recognizer));
    } else {
     printf("%s\n", vosk_recognizer_partial_result(recognizer));
    }
}

void recognizeWavFile(VoskRecognizer *recognizer, char *filename){
    FILE *wavin;
    char buffer[BLOCK_SIZE];
    int nread;
    wavin = fopen(filename, "rb");
    fseek(wavin, 44, SEEK_SET); // offset 44 bytes of wav header
    while(!feof(wavin)){
        nread = fread(buffer, 1, sizeof(buffer), wavin);
        dequeue_audio_block();
        recognizeAudioBlock(recognizer, buffer, sizeof(buffer));
    } 
    fclose(wavin);
}