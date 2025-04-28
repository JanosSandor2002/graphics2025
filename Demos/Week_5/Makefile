CC = gcc
CFLAGS = -Iinclude/ -Wall -Wextra -Wpedantic
LDFLAGS = -lopengl32 -lglu32 -lm -lSDL2 -lSDL2_image

SRCS = src/app.c src/camera.c src/main.c src/scene.c src/utils.c
OBJS = src/app.o src/camera.o src/main.o src/scene.o src/utils.o
TARGET = origin.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGET)
