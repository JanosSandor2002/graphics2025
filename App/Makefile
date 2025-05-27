CC = gcc

INCLUDES = -Iinclude -Isoil -IMinGW/include
LIBS = -LMinGW/lib -lSDL2 -lSDL2main soil/libSOIL.a -lopengl32 -lglu32

CAMERA_OBJ = src/Camera.o
MODEL_OBJ = src/Model.o
FOG_OBJ = src/Fog.o
APP_OBJ = src/App.o
MAIN_OBJ = src/main.o

TARGET = main.exe

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(APP_OBJ) $(CAMERA_OBJ) $(MODEL_OBJ) $(FOG_OBJ)
	$(CC) $^ -o $@ $(LIBS)

src/%.o: src/%.c
	$(CC) $(INCLUDES) -c $< -o $@

clean:
	del /Q src\*.o main.exe 2>nul || exit 0