CC = gcc
CFLAGS = -Iinclude -Isoil -Ifreeglut -IC:/Users/User/Desktop/programing/c_sdk_220203/MinGW/include
LDFLAGS = -Lsoil -Lfreeglut -lopengl32 -lglu32 -lsoil -lfreeglut

SOIL_OBJ = soil/soil.o soil/image_dxt.o soil/image_helper.o
FREEGLUT_OBJ = freeglut/stb_image_aug.o
CAMERA_OBJ = src/Camera.o
MODEL_OBJ = src/Model.o
FOG_OBJ = src/Fog.o

MAIN_OBJ = src/main.o
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(CAMERA_OBJ) $(SOIL_OBJ) $(FREEGLUT_OBJ) $(MODEL_OBJ) $(FOG_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(MAIN_OBJ): src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

$(CAMERA_OBJ): src/Camera.c src/Camera.h
	$(CC) $(CFLAGS) -c src/Camera.c -o src/Camera.o

$(MODEL_OBJ): src/Model.c src/Model.h
	$(CC) $(CFLAGS) -c src/Model.c -o src/Model.o

$(FOG_OBJ): src/Fog.c src/Fog.h
	$(CC) $(CFLAGS) -c src/Fog.c -o src/Fog.o

soil/%.o: soil/%.c
	$(CC) $(CFLAGS) -c $< -o $@

freeglut/%.o: freeglut/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o main.exe soil\*.o freeglut\*.o 2>nul || exit 0