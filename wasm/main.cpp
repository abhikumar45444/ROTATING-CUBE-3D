#include<iostream>
#include "../include/raylib.h"
#include "../include/raymath.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

using namespace std;

int width = 1000;
int height = 700;

#define GRID_COUNT 15
#define GRID_PAD 30
#define GRID_SIZE ((GRID_COUNT - 1) * GRID_PAD)

//! CIRCLE
#define RADIUS 10
Color backgroundColor = {22, 22, 22, 255};
Color circleColor = {200, 30, 30, 255};
float innerRadius = RADIUS + 1;
float outerRadius = RADIUS + 3;
//! CIRCLE

//! TRIANGLE
float triWide = 10.0f;
float triHeight = 15.0f;
//! TRIANGLE


//! CUBE STRUCTURE
typedef struct Point{
    float x;
    float y;
    float z;
    float radius;
    Color color;
} Point;

typedef struct Cube{
    Point points[GRID_COUNT*GRID_COUNT*GRID_COUNT];
}Cube;
//! CUBE STRUCTURE


//! MENU 
typedef enum Shape
{
    CIRCLE,
    RING,
    TRIANGLE
} Shape;

typedef struct Text{
    const char *str;
    int posX;
    int posY;
    int fontSize;
    Color textColor;
}Text;

typedef struct CheckBox{
    Rectangle chBxrect;
    Text text;
    Color chBxcolor;
    bool chBxselected;
    Shape chBxshape;
}CheckBox;

void DrawCheckBoxShapes(CheckBox *checkBoxes, int size);
void UserInputCheckBoxShapes(CheckBox *checkBoxes, int size);
//! MENU 

void UpdateDrawFrame(void);

int main()
{
    const char *title = "3D Cube Projection On 2D Plane";
    SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(width, height, title);
    const int FPS = 60;
    SetTargetFPS(FPS);

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
        while (!WindowShouldClose())
        {
            UpdateDrawFrame();
        }
    #endif

    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void)
{
    static Cube cube = {};

    static float angle = 0;
    static float x = 0.0f;
    static float y = 0.0f;
    static float z = 0.0f;
    static float zeye = 320.0f;

    // point of rotation
    static float xp = 0;
    static float yp = 0;
    static float zp = (GRID_SIZE * 0.5f + zeye) / GRID_COUNT;
    static int idx = 0;

    if (idx < 1)
    {
        for (int iy = 0; iy < GRID_COUNT; iy++)
        {
            for (int ix = 0; ix < GRID_COUNT; ix++)
            {
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
                    z = ((iz * GRID_PAD) + zeye) / GRID_COUNT;

                    cube.points[idx].x = x;
                    cube.points[idx].y = y;
                    cube.points[idx].z = z;
                    cube.points[idx].radius = RADIUS;
                    cube.points[idx].color = circleColorGradient;
                    idx++;
                }
            }
        }
    }

    //! MENU INIT
    static float chBxInitX = 20.0f, chBxInitY = 50.0f, chBxWidth = 20.0f, chBxHeight = 20.0f, chBxYoffset = 30.0f;
    static int chBxTextInitX = chBxInitX + 30.0f, chBxTextInitY = chBxInitY + 3.0f, fontSize = 15.0f, chBxTextYOffset = chBxYoffset;

    static CheckBox circleChBx = {
        {chBxInitX, chBxInitY, chBxWidth, chBxHeight},
        {"Circle", chBxTextInitX, chBxTextInitY, fontSize, RAYWHITE},
        RED,
        true,
        CIRCLE,
    };

    static CheckBox ringChBx = {
        {chBxInitX, chBxInitY + chBxYoffset, chBxWidth, chBxHeight},
        {"Ring", chBxTextInitX, chBxTextInitY + chBxTextYOffset, fontSize, RAYWHITE},
        WHITE,
        false,
        RING,
    };

    static CheckBox triangleChBx = {
        {chBxInitX, chBxInitY + 2.0f * chBxYoffset, chBxWidth, chBxHeight},
        {"Triangle", chBxTextInitX, chBxTextInitY + 2 * chBxTextYOffset, fontSize, RAYWHITE},
        WHITE,
        false,
        TRIANGLE,
    };

    static CheckBox checkBoxes[] = {circleChBx, ringChBx, triangleChBx};
    unsigned int checkBoxesLength = sizeof(checkBoxes) / sizeof(CheckBox);
    //! MENU INIT

    angle += 3 * PI * GetFrameTime();
    BeginDrawing();
    ClearBackground(backgroundColor);
    //! MENU
    UserInputCheckBoxShapes(checkBoxes, checkBoxesLength);

    Shape selectedShape;

    for (int i = 0; i < checkBoxesLength; i++)
    {
        if (checkBoxes[i].chBxselected)
        {
            selectedShape = checkBoxes[i].chBxshape;
        }
    }

    DrawText("Select Filling Shape Of Cube : ", chBxInitX, chBxInitY * 0.5f, 15, RAYWHITE);
    DrawCheckBoxShapes(checkBoxes, checkBoxesLength);

    // * TODO: implement selection for different shapes - DONE ✓
    // * TODO: implement slider for zeye -
    // * TODO: implement slider for circle and ring radius -
    // * TODO: implement slider for triangle height and span (width) -

    //! MENU

    for (int i = 0; i < GRID_COUNT * GRID_COUNT * GRID_COUNT; i++)
    {
        // translate to origin - for rotation
        float xPrime = cube.points[i].x - xp;
        float yPrime = cube.points[i].y - yp;
        float zPrime = cube.points[i].z - zp;

        // performing rotation - for rotation
        float newX = xPrime * cosf(DEG2RAD * angle) + zPrime * sinf(DEG2RAD * angle);
        float newY = yPrime;
        float newZ = -xPrime * sinf(DEG2RAD * angle) + zPrime * cosf(DEG2RAD * angle);

        // translate back to its position - for rotation
        float x = newX + xp;
        float y = newY + yp;
        float z = newZ + zp;

        // perspective divide
        x = x / z;
        y = y / z;

        x = (x + 1) / 2;
        y = (y + 1) / 2;

        // scaling the cordinates back
        x *= width;
        y *= height;

        switch (selectedShape)
        {
            case CIRCLE:
            {
                Color circleColor = cube.points[i].color;

                float rad = cube.points[i].radius / z;
                rad *= cube.points[i].radius;
                DrawCircle(x, y, rad, circleColor);
                break;
            }

            case RING:
            {
                Color ringColor = cube.points[i].color;

                float _innerRadius = innerRadius;
                float _outerRadius = outerRadius;
                float inrad = _innerRadius / z;
                float outrad = _outerRadius / z;
                inrad *= _innerRadius;
                outrad *= _outerRadius;
                DrawRing({x, y}, inrad, outrad, 0, 360, 10, ringColor);
                break;
            }

            case TRIANGLE:
            {
                Color triangColor = cube.points[i].color;

                Vector2 v1 = {};
                Vector2 v2 = {};
                Vector2 v3 = {};

                float triW = triWide / z;
                float triH = triHeight / z;

                triW *= triWide;
                triH *= triHeight;

                v1.x = x;
                v1.y = y;

                v2.x = x - triW;
                v2.y = y + triH;

                v3.x = x + triW;
                v3.y = y + triH;

                DrawTriangleLines(v1, v2, v3, triangColor);
                break;
            }

            default:
            {
                const char *defaultText = "SWITCH DEFAULT EXECUTED !! \n\t\t\tNO SHAPE DETECTED ..";
                int fontSize = 20;
                int defaultTextWidth = MeasureText(defaultText, fontSize);
                DrawText(defaultText, width * 0.5f - defaultTextWidth * 0.5f, height * 0.5f, fontSize, WHITE);
                break;
            }
        }
    }
    EndDrawing();
}


void DrawCheckBoxShapes(CheckBox *checkBoxes, int size)
{
    float roundness = 0.5f;
    float segments = 10.0f;
    for (int i = 0; i < size; i++)
    {
        Rectangle rect1 = checkBoxes[i].chBxrect;
        DrawRectangleRounded(rect1, roundness, segments, checkBoxes[i].chBxcolor);
        DrawText(checkBoxes[i].text.str, checkBoxes[i].text.posX, checkBoxes[i].text.posY, checkBoxes[i].text.fontSize, checkBoxes[i].text.textColor);
    }
}


void UserInputCheckBoxShapes(CheckBox *checkBoxes, int size)
{
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < size; i++)
    {
        if ((mousePos.x >= checkBoxes[i].chBxrect.x && mousePos.x <= checkBoxes[i].chBxrect.x + checkBoxes[i].chBxrect.width) && (mousePos.y >= checkBoxes[i].chBxrect.y && mousePos.y <= checkBoxes[i].chBxrect.y + checkBoxes[i].chBxrect.height))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                checkBoxes[i].chBxcolor = RED;
                checkBoxes[i].chBxselected = true;
                for (int j = 0; j < size; j++)
                {
                    if((checkBoxes[j].chBxselected) && (i != j))
                    {
                        checkBoxes[j].chBxcolor = WHITE;
                        checkBoxes[j].chBxselected = false;
                    }
                }
            }
        }
    }
    
}