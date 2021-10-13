# for information on compiling youself, look for:
# flags, paths, links and source files

## flags
CXX_FLAGS = -Wshadow -Wall -std=c++17
CXX_RELEASE_FLAGS = -O2 -DGLEW_STATIC
CXX_DEBUG_FLAGS = -g -fsanitize=undefined -D_GLIBCXX_DEBUG -DGLEW_STATIC
#CXX_DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG -DGLEW_STATIC

## paths
SRC_PATH = ./src/
INCLUDE_PATH = ./include/

## links
LINKS = -L ./bin/GLFW/ -lGL -lglfw3 -ldl -lpthread -L ./bin/GLEW/ -lglew

## source files
SRC_FILES = \
$(SRC_PATH)driver.cpp \
$(SRC_PATH)callbacks.cpp \
$(SRC_PATH)camera.cpp \
$(SRC_PATH)shader_program.cpp \
$(SRC_PATH)wave_object.cpp \
$(SRC_PATH)light_object.cpp

## default
all: release

## dependencies
deps:
	cd ./vendor/ && make glfw
	cd ./vendor/ && make glew
	cd ./vendor/ && make glm
	git submodule deinit -f .
	git submodule update --init

## debug
debug: debug_prep debug_compile clean print_done

debug_prep:
	if [ ! -d "./int" ]; then mkdir ./int; fi;
	rm -rf ./int/*;
	if [ ! -d "./bin" ]; then mkdir ./bin; fi;

debug_compile:
	g++ $(CXX_FLAGS) $(SRC_FILES) -o driver_debug $(LINKS) $(CXX_DEBUG_FLAGS) -I $(INCLUDE_PATH)

## release
release: release_prep release_compile clean print_done

release_prep:
	if [ ! -d "./int" ]; then mkdir ./int; fi;
	rm -rf ./int/*;
	if [ ! -d "./bin" ]; then mkdir ./bin; fi;

release_compile:
	g++ $(CXX_FLAGS) $(SRC_FILES) -o driver_release $(LINKS) $(CXX_RELEASE_FLAGS) -I $(INCLUDE_PATH)

## clean up
clean:
	if [ -d "./int" ]; then rm -rf ./int; fi;

## print done
print_done:
	@echo "done"
