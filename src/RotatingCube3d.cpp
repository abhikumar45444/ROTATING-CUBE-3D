#include<iostream>
#include "../include/raylib.h"
#include "../include/raymath.h"
using namespace std;

int width = 1000;
int height = 800;

#define GRID_COUNT 15
#define GRID_PAD 30
#define GRID_SIZE ((GRID_COUNT - 1) * GRID_PAD)
#define RADIUS 10
Color backgroundColor = {22, 22, 22, 255};
Color circleColor = {200, 30, 30, 255};
float innerRadius = 11;
float outerRadius = 13;

int main()
{
    const char *title = "3D object projection on 2D plane";
    SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(width, height, title);
    SetTargetFPS(60);

    float angle = 0;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float zeye = 300.0f;

    // point of rotation
    float xp = 0;
    float yp = 0;
    float zp = (GRID_SIZE * 0.5f + zeye) / GRID_COUNT; 

    while(!WindowShouldClose())
    {
        angle += 3 * PI * GetFrameTime();
        BeginDrawing();
        ClearBackground(backgroundColor);       
        for (int iy = 0; iy < GRID_COUNT; iy++)
        {
            for (int ix = 0; ix < GRID_COUNT; ix++)
            {
                // float radius = RADIUS;
                float _innerRadius = innerRadius;
                float _outerRadius = outerRadius;
                for (int iz = 0; iz < GRID_COUNT; iz++)
                {
                    unsigned int r = ((iz < 8) && (iy < 8) && (ix < 8)) ? (((ix + 2) * 255) / GRID_COUNT) : ((ix * 255) / GRID_COUNT);
                    unsigned int g = ((iz < 8) && (iy < 8) && (ix < 8)) ? (((iy + 2) * 255) / GRID_COUNT) : ((iy * 255) / GRID_COUNT);
                    unsigned int b = ((iz < 8) && (iy < 8) && (ix < 8)) ? (((iz + 2) * 255) / GRID_COUNT) : ((iz * 255) / GRID_COUNT);

                    unsigned int color = 0x000000ff | (r << 24) | (g << 16) | (b << 8);
                    Color circleColorGradient = GetColor(color);

                    // calculating the object position
                    x = (ix * GRID_PAD - (GRID_SIZE * 0.5f)) / GRID_COUNT;
                    y = (iy * GRID_PAD - (GRID_SIZE * 0.5f)) / GRID_COUNT;
                    z =(( iz * GRID_PAD) + zeye) / GRID_COUNT;

                    // translate to origin - for rotation
                    float xPrime = x - xp ;
                    float yPrime = y - yp ;
                    float zPrime = z - zp ;

                    // performing rotation - for rotation
                    float newX = xPrime * cosf(DEG2RAD * angle) + zPrime * sinf(DEG2RAD * angle);
                    float newY = yPrime;
                    float newZ = -xPrime * sinf(DEG2RAD * angle) + zPrime * cosf(DEG2RAD * angle);

                    // translate back to its position - for rotation
                    x = newX + xp ;
                    y = newY + yp ;
                    z = newZ + zp ;
                    
                    // perspective divide
                    x = x / z;
                    y = y / z;

                    x = (x + 1) / 2;
                    y = (y + 1) / 2;

                    // scaling the cordinates back
                    x *= width; 
                    y *= height;

                    // float rad = radius / z;
                    // rad *= RADIUS;
                    float inrad = _innerRadius / z;
                    float outrad = _outerRadius / z;
                    inrad *= innerRadius;
                    outrad *= outerRadius;

                    // DrawCircle(x , y, rad, circleColorGradient);
                    DrawRing({x , y}, inrad, outrad, 0, 360, 10, circleColorGradient);
                    DrawFPS(10,10);
                    // DrawTriangle()
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}