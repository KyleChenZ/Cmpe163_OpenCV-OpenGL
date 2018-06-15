# Cmpe163_OpenCV-OpenGL
Computer graphic with image processing (camera)

In this project, video streaming with raw images and line detecting images are shown. Opencv is used to transform video images frame to Opengl pixels. Moreover, Opengl is used to draw the cursor, light reflection, and the cubes.

Demo video on Youtube:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=FasX8Iwam7k
" target="_blank"><img src="http://img.youtube.com/vi/FasX8Iwam7k/0.jpg" 
alt="OpenCV+OpenGL" width="240" height="180" border="10" /></a>

As you can see from the video, when the cursor is pointing to the left cube, raw images will be shown. When the cursor is pointing to the right cube, images with line detection will be shown. Moreover, the light reflection on the cursor is constantly changing based on its position related to the light source in the program.

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
