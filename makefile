## flags
CXX_FLAGS = -Wshadow -Wall -std=c++17
CXX_RELEASE_FLAGS = -O2 -DGLEW_STATIC
CXX_DEBUG_FLAGS = -g -fsanitize=undefined -D_GLIBCXX_DEBUG -DGLEW_STATIC
#CXX_DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG -DGLEW_STATIC

## paths
SRC_PATH = ./src/
INCLUDE_PATH = ./include/

## source files
SRC_FILES = \
$(SRC_PATH)driver.cpp \
$(SRC_PATH)callbacks.cpp \
$(SRC_PATH)camera.cpp \
$(SRC_PATH)shader_program.cpp

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
	g++ $(CXX_FLAGS) $(SRC_FILES) -o driver_debug \
	-L ./bin/GLFW/ -lGL -lglfw3 -ldl -lpthread -L ./bin/GLEW/ -lglew $(CXX_DEBUG_FLAGS) \
	-I $(INCLUDE_PATH)
	if [ ! -f "driver_debug" ]; then mv driver_debug ./bin/driver_debug; fi;

## release
release: release_prep release_compile clean print_done

release_prep:
	if [ ! -d "./int" ]; then mkdir ./int; fi;
	rm -rf ./int/*;
	if [ ! -d "./bin" ]; then mkdir ./bin; fi;

release_compile:
	g++ $(CXX_FLAGS) $(SRC_FILES) -o driver_release \
	-L ./bin/GLFW/ -lGL -lglfw3 -ldl -lpthread -L ./bin/GLEW/ -lglew $(CXX_RELEASE_FLAGS) \
	-I $(INCLUDE_PATH)
	if [ ! -f "driver_release" ]; then mv driver_release ./bin/driver_release; fi;

## clean up
clean:
	if [ -d "./int" ]; then rm -rf ./int; fi;

## print done
print_done:
	@echo "done"
