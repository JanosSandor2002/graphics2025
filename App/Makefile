CC = gcc

INCLUDES = -Iinclude -Isoil -Ifreeglut -IC:/Users/User/Desktop/programing/c_sdk_220203/MinGW/include
LIBS = -LC:/SDL2/lib -lSDL2 -lSDL2main soil/libSOIL.a -lopengl32 -lglu32

CAMERA_OBJ = src/Camera.o
MODEL_OBJ = src/Model.o
FOG_OBJ = src/Fog.o
MAIN_OBJ = src/main.o

TARGET = main.exe

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(CAMERA_OBJ) $(MODEL_OBJ) $(FOG_OBJ)
	$(CC) $^ -o $@ $(LIBS)

src/%.o: src/%.c
	$(CC) $(INCLUDES) -c $< -o $@

clean:
	del /Q src\*.o main.exe 2>nul || exit 0