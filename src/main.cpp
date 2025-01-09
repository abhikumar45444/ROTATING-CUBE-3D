#include "../include/raylib.h"
#include "../include/raymath.h"
#include <iostream>
#include <ctime>

using namespace std;

int width = 800;
int height = 800;

Color colorLerp(Color color1, Color color2, float amount)
{
    unsigned char r = color1.r + amount*(color2.r - color1.r);
    unsigned char g = color1.g + amount*(color2.g - color1.g);
    unsigned char b = color1.b + amount*(color2.b - color1.b);
    return CLITERAL(Color){r, g, b, 255};
}

int main()
{
    const char *title = "interpolation";
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(width, height, title);
    SetTargetFPS(60);
    Color color = RED;
    float value = 1.0f;
    while(!WindowShouldClose())
    {
        width = GetScreenWidth();
        height = GetScreenHeight();

        BeginDrawing();
        ClearBackground(BLACK);
        float dt = GetFrameTime();
        float alpha = (color.a - dt)/255.0f;
        // float alpha = (color.a - dt)/255.0f;
        // TraceLog(LOG_INFO, "%.2f - %.2f , %.2f, %.2f", (float)color.a, dt,  (float)(color.a - dt)/255, alpha);

        color = Fade(color, alpha);

        // DrawRectangleGradientH(0, 0, width, height, RED, BLUE);
        if(value <= 0)
            value = 1.0f;
        value = value - dt;

        Vector2 mousePos = GetMousePosition();
        color = colorLerp(RED, BLUE, mousePos.x/width);
        // color = colorLerp(RED, BLUE, value);

        DrawRectangle(0, 0, width, height, color);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}