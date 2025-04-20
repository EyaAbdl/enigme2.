CC = gcc
CFLAGS = -Wall -Wextra
TARGET = prog


SDL_CFLAGS = -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
SDL_LIBS = -lSDL -lSDL_image -lSDL_ttf

all: $(TARGET)

$(TARGET): main.c source.c header.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) main.c source.c -o $(TARGET) $(SDL_LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
