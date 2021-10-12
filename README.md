# Surface-waves
# Setup
### Debian/Ubuntu
##### glew github page specifies some additional required tools:

	$ sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev
##### Clone with submodules and build:

	$ git clone --recurse-submodules https://github.com/Slemmie/surface-waves.git
	$ cd surface-waves
	$ make deps
	$ make
##### Execute:

	$ ./driver_release
##### Build and execute in debug mode:

	$ make debug
	$ ./driver_debug

### Any other system
Take a look at glew and glfw github pages for more information.

Copy glm files with `$ cd vendor/ && make glm`.

glew and glfw static libs should be located in `bin/GLEW/libglew.a` and `bin/GLFW/libglfw3.a` respectively.

glew and glfw headers should be located in `include/GLEW/glew.h` and `include/GLFW/glfw3.h` respectively.

The regular `$ make` after dependencies are sorted might only work on some systems. Compilation flags and the like are found in the `makefile` file.
# Submodules
Submodules are found in `vendor/`.

[glew](https://github.com/nigels-com/glew)

[glfw](https://github.com/glfw/glfw)

[glm](https://github.com/g-truc/glm)
