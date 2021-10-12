# make/export deps

## glfw
glfw_src_files = \
./glfw/src/context.c ./glfw/src/init.c ./glfw/src/input.c \
./glfw/src/monitor.c ./glfw/src/vulkan.c ./glfw/src/window.c \
./glfw/src/x11_init.c ./glfw/src/x11_monitor.c ./glfw/src/x11_window.c \
./glfw/src/xkb_unicode.c ./glfw/src/posix_time.c ./glfw/src/posix_thread.c \
./glfw/src/glx_context.c ./glfw/src/egl_context.c ./glfw/src/osmesa_context.c \
./glfw/src/linux_joystick.c
.PHONY: glfw
glfw:
	if [ ! -d "../bin/GLFW/" ]; then mkdir -p ../bin/GLFW/; fi;
	gcc -Wall -c -fPIC $(glfw_src_files) -O2 -D_GLFW_X11 \
	-I ./glfw/include/ -I ./glfw/src/ -I ./glfw/
	ar cr ../bin/GLFW/libglfw3.a *.o
	rm *.o
	if [ ! -d "../include/GLFW/" ]; then mkdir -p ../include/GLFW/; fi;
	cp ./glfw/include/GLFW/glfw3.h ../include/GLFW/glfw3.h
	@echo "done"

## glew
.PHONY: glew
glew:
	cd ./glew/auto/ && make
	cd ./glew/ && make && sudo make install
	if [ ! -d "../bin/GLEW/" ]; then mkdir -p ../bin/GLEW/; fi;
	cp ./glew/lib/libGLEW.a ../bin/GLEW/libglew.a
	if [ ! -d "../include/GLEW/" ]; then mkdir -p ../include/GLEW/; fi;
	cp ./glew/include/GL/glew.h ../include/GLEW/glew.h
	cd ./glew/ && make clean
	@echo "done"

## glm
.PHONY: glm
glm:
	if [ ! -d "../include/glm/" ]; then mkdir -p ../include/glm/; fi;
	cp -r ./glm/glm/* ../include/glm/
