# Cmpe163_OpenCV-OpenGL
computer graphic with image processing (camera)

Demo video on Youtube:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=FasX8Iwam7k
" target="_blank"><img src="http://img.youtube.com/vi/FasX8Iwam7k/0.jpg" 
alt="OpenCV+OpenGL" width="240" height="180" border="10" /></a>

Opencv3 has to be installed in order to compile the files and run them

The programs were only tested on Ubuntu16.04

To compile reflection.c:

`gcc -o reflection reflection.c -lGL -lGLU -lglut -lm -std=gnu11`

reflection.c integrate both opengl and opencv

To run the program:

`./reflection`

To compile camera.cpp:

```g++ -std=c++11 camera.cpp `pkg-config --libs --cflags opencv` -o camera```

camera.cpp contains opencv programming

To run the program:

`./camera`

To compile cube.cpp:

`gcc -o cube cube.cpp -lGL -lGLU -lglut -lm -std=gnu11`

cube.cpp contains opengl programming

To run the program:

`./cube`
