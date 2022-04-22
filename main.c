#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "recognition.h"

#define true 1
#define NUM_THREADS 2

int main(){
    queue_size = 0;
	pthread_t thread_pool[NUM_THREADS];
    printf("[+] Initializing model...\n");
    VoskModel *model = vosk_model_new("model");
    VoskRecognizer *recognizer = vosk_recognizer_new(model, SAMPLERATE);
    int final;
	printf("[+] Initializing thread...\n");
    char filename[100] = "test_comando.wav";
    int rc = pthread_create(&thread_pool[1], NULL, getBlockFromMic, NULL);
	//int rc = pthread_create(&thread_pool[1], NULL, getBlockFromFile, (void*)filename);
    while(true){
        struct audio_block *block = dequeue_audio_block();
        if(block != NULL){
            analyzeFromBuffer(recognizer, block->data, block->size);
        }
    }
	printf("Final result %s\n", vosk_recognizer_final_result(recognizer));

	printf("Releasing memory...\n");
    vosk_recognizer_free(recognizer);
    vosk_model_free(model);
    
    pthread_exit(NULL);
	return 0;
}
