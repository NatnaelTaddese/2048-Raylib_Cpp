# Define variables for include and library paths
INCLUDE_PATH = ./include/
LIBRARY_PATH = ./lib/
BUILD_PATH = ./build/

# Define platform-specific variables
ifeq ($(OS),Windows_NT)
    INCLUDE_PATH = ./include/platform/windows_64/
    LIBRARY_PATH = ./lib/platform/windows_64/
    LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    BUILD_PATH = ./build/windows_64/
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        INCLUDE_PATH = ./include/platform/linux_64/
        LIBRARY_PATH = ./lib/platform/linux_64/
        LIBS = -lraylib -lGL -lm -lpthread -ldl
        BUILD_PATH = ./build/linux_64/
    endif
    ifeq ($(UNAME_S),Darwin)
        INCLUDE_PATH = ./include/platform/macos/
        LIBRARY_PATH = ./lib/platform/macos/
        LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        BUILD_PATH = ./build/macos/
    endif
endif

# Default target
default:
	g++ ./src/*.cpp -o $(BUILD_PATH) -O2 -Wall -Wno-missing-braces -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) $(LIBS)


#default:
#	g++ ./src/*.cpp -o ./build/Game.exe -O2 -Wall -Wno-missing-braces -I ./include/ -L ./lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
#
## default:
## 	g++ ../src/*.cpp -o Game.exe -O2 -Wall -Wno-missing-braces -I ../include/ -L ../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm