CC = gcc
CFLAGS = -Iinclude/ -Wall -Wextra -Wpedantic -Dmain=SDL_main
LDFLAGS = -lopengl32 -lglu32 -lm -lSDL2main -lSDL2 -lSDL2_image

SRCS = src/app.c src/camera.c src/main.c src/scene.c src/utils.c
OBJS = $(SRCS:.c=.o)

TARGET = origin.exe

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and target
clean:
	rm -f $(OBJS) $(TARGET)
