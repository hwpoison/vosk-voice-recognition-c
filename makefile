CC=gcc
CFLAGS= -llibvosk -lwinmm -lpthread  -L.
TARGET=vosk_recognizer.exe

all: voski 

voski:	record_audio.o wavutils.o recognition.o
	$(CC) -g main.c record_audio.o wavutils.o recognition.o -o $(TARGET) $(CFLAGS) 

clean:
	del *.o
