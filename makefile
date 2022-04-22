CC=gcc
CFLAGS= -llibvosk -lwinmm -L.
TARGET=vosk_recogizer.exe

all: voski 

voski:	record_audio.o wavutils.o recognition.o
	$(CC) main.c record_audio.o wavutils.o recognition.o -o $(TARGET) $(CFLAGS)

clean:
	del *.o