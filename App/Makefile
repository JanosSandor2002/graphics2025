CC = gcc
CFLAGS = -I. -Isoil -Ifreeglut -IC:/Users/User/Desktop/programing/c_sdk_220203/MinGW/include
LDFLAGS = -Lsoil -Lfreeglut -lopengl32 -lglu32 -lsoil -lfreeglut

SOIL_OBJ = soil/soil.o soil/image_dxt.o soil/image_helper.o
FREEGLUT_OBJ = freeglut/stb_image_aug.o
CAMERA_OBJ = Camera.o

MAIN_OBJ = main.o
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(CAMERA_OBJ) $(SOIL_OBJ) $(FREEGLUT_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(MAIN_OBJ): main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

$(CAMERA_OBJ): Camera.c Camera.h
	$(CC) $(CFLAGS) -c Camera.c -o Camera.o

soil/%.o: soil/%.c
	$(CC) $(CFLAGS) -c $< -o $@

freeglut/%.o: freeglut/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q main.o main.exe 2>nul || exit 0
