# ROTATING CUBE OF CIRCLES 3D (PROJECTION) IN C++ USING RAYLIB

This project visualizes a 3D rotating cube represented by circles at its vertices, rendered on a 2D plane. The rotation is achieved by projecting the 3D cube's vertices onto a 2D canvas and applying transformations for smooth rotation along all three axes (X, Y, and Z).

- The program demonstrates concepts like:

    - 3D geometry and transformations
    - Projection of 3D points onto 2D space
    - Rotation using trigonometric functions
 
 ![thumbnail](./RotatingCube3d.PNG)

## ROTATING CUBE 3D PREVIEW


## DESCRIPTION
 A player moves the paddle back and forth horizontally to hit and ricochet the ball to cause it to break the bricks on top of the screen.
 If the ball reaches the bottom of the screen it costs one life.
 The bricks are in color of yellow, green, orange, and red from bottom to top.


## SEE CUBE ON WEB
Checkout Game Here: [RotatingCube3d](#).

## (WINDOWS - EXE)
 Run the [RotatingCube3d.exe](./build) file.

## BUILD: If you want to make changes feel free to do so
***
Open Makefile
Make necessary adjustments like Compiler for raylib c++ <code>[raylib github windows setup](https://github.com/raysan5/raylib/wiki/Working-on-Windows)</code>.</br>
You can find raylib library in <code>[library folder](./lib)</code> or else you need to compile the library by yourself.
***

## DEPENDENCIES
```console
raylib
opengl32 
gdi32 
winmm
```

## REFERENCES
[3D Projection](https://en.wikipedia.org/wiki/3D_projection) </br>
[Perspective Projection](https://www.geeksforgeeks.org/perspective-projection-and-its-types/) </br>
[Rotation Matrix](https://en.wikipedia.org/wiki/Rotation_matrix) </br>
