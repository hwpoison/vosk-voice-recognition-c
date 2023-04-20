#include "recognition.h"

#define true 1

void enqueue_audio_block(audio_block *data)
{
    if (queue_size == 0)
    {
        queue_head = data;
        queue_tail = data;
    }
    else
    {
        queue_tail->next = data;
        queue_tail = data;
    }
    queue_size++;
}

audio_block *dequeue_audio_block()
{
    if (queue_size == 0)
    {
        return NULL;
    }
    audio_block *data = queue_head;
    queue_head = queue_head->next;
    queue_size--;
    return data;
}


void initializeModelAndRecognizer(){
	gvosk_model = vosk_model_new("model");
	if(gvosk_model == NULL){
	    fprintf(stderr, "[x] ERROR TO LOAD MODEL, CHECK IF EXISTS IN THE FOLDER\n");
	    exit(-1);
    	}
	gvosk_recognizer = vosk_recognizer_new(gvosk_model, wav.sampleRate);
}

void *getBlockFromMic()
{
    audio_block *block = malloc(sizeof(audio_block));
    block->size = BLOCK_SIZE;
    block->data = malloc(sizeof(char) * block->size);
    while (true)
    {
        record(block->data, block->size);
        enqueue_audio_block(block);
    }
}

recognition_result *recognizeAudioBlock(char *data, int nlen)
{
    int final = vosk_recognizer_accept_waveform(gvosk_recognizer, data, nlen);
    recognition_result *result = malloc(sizeof(struct result_t));
    if (final)
    {
        result->content = (char*)vosk_recognizer_result(gvosk_recognizer);
	result->type = ENTIRE_RESULT;
    }
    else
    {
	result->content = (char*)vosk_recognizer_partial_result(gvosk_recognizer);
	result->type = PARTIAL_RESULT;
    }
    return result;
}

void recognizeFromAudioBlockQueue(char *output_filename) {
        audio_block *audio_queue_head;
	recognition_result *result = malloc(sizeof(struct result_t*));
	while (true)
	{
	    audio_queue_head = dequeue_audio_block();
	    if(audio_queue_head != NULL){
		result = recognizeAudioBlock(audio_queue_head->data, audio_queue_head->size);
		printf("%s\n", result->content);
		
		// when recognition is finished and it has content, reset recognizer
		if(result->type == ENTIRE_RESULT && !strstr(result->content, "\"\"")){
			printf("%s\n", result->content);
			writeToFile(output_filename, result->content, strlen(result->content));
			vosk_recognizer_free(gvosk_recognizer);
			gvosk_recognizer = vosk_recognizer_new(gvosk_model, wav.sampleRate);
		}
	    }
	}
}

void recognizeWavFile(char *input_audio_fp, char *output_filename)
{
    FILE *wavin;
    char buffer[BLOCK_SIZE];
    int nread;
    wavin = fopen(input_audio_fp, "rb");
    fseek(wavin, 44, SEEK_SET); // offset 44 bytes to omits wav header
				//
    recognition_result *result = malloc(sizeof(struct result_t));

    while (!feof(wavin))
    {
        nread = fread(buffer, 1, BLOCK_SIZE, wavin);
        result = recognizeAudioBlock(buffer, BLOCK_SIZE);
	if(result->type == ENTIRE_RESULT){
		printf("%s\n", result->content);
		writeToFile(output_filename, result->content, strlen(result->content));
		vosk_recognizer_free(gvosk_recognizer);
		gvosk_recognizer = vosk_recognizer_new(gvosk_model, wav.sampleRate);
	}
    }
    unsigned char* finalResult = getFinalResult(gvosk_recognizer);
    printf("%s\n", finalResult);
    writeToFile(output_filename, finalResult, strlen(finalResult));
    
    printf("[+] EOF wav.\n");
    fclose(wavin);
}

unsigned char *getFinalResult()
{
    return (unsigned char *)vosk_recognizer_result(gvosk_recognizer);
}

int checkFileExists(char *filename)
{
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }
    else
    {
        fclose(file);
        return 1;
    }
}

void writeToFile(char *filename, char *data, int nlen)
{
    FILE *file;
    file = fopen(filename, "a");
    if (file == NULL)
    {
        fprintf(stderr, "[x] Error opening file\n");
    }
    else
    {
        fwrite(data, sizeof(char), nlen, file);
        fclose(file);
    }
}
