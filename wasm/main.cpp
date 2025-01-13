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
    SHAPE_NONE,
    CIRCLE,
    RING,
    TRIANGLE
} Shape;

typedef enum Rotation
{
    ROTATION_NONE,
    X_AXIS,
    Y_AXIS,
    Z_AXIS
} Rotation;

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
    Rotation chBxRot;
}CheckBox;

Font font;
int fontSize = 25;
float fontSpacing = 2.0f;

void DrawCheckBox(CheckBox *checkBoxes, int size);
void UserInputCheckBox(CheckBox *checkBoxes, int size);
//! MENU 

void UpdateDrawFrame(void);

int main()
{
    const char *title = "3D Cube Projection On 2D Plane";
    SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(width, height, title);
    const int FPS = 60;
    SetTargetFPS(FPS);

    // Load the custom font
    const char *fontName = "../resources/font/Poppins.otf";
    font = LoadFontEx(fontName, fontSize, nullptr, 0);

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
    static int chBxTextInitX = chBxInitX + 30.0f, chBxTextInitY = chBxInitY - 1.0f, /* fontSize = 15.0f, */ chBxTextYOffset = chBxYoffset;

    // Shape Filling CheckBox
    static CheckBox circleChBx = {
        {chBxInitX, chBxInitY, chBxWidth, chBxHeight},
        {"Circle", chBxTextInitX, chBxTextInitY, fontSize, RAYWHITE},
        RED,
        true,
        CIRCLE,
        ROTATION_NONE
    };

    static CheckBox ringChBx = {
        {chBxInitX, chBxInitY + chBxYoffset, chBxWidth, chBxHeight},
        {"Ring", chBxTextInitX, chBxTextInitY + chBxTextYOffset, fontSize, RAYWHITE},
        WHITE,
        false,
        RING,
        ROTATION_NONE
    };

    static CheckBox triangleChBx = {
        {chBxInitX, chBxInitY + 2.0f * chBxYoffset, chBxWidth, chBxHeight},
        {"Triangle", chBxTextInitX, chBxTextInitY + 2 * chBxTextYOffset, fontSize, RAYWHITE},
        WHITE,
        false,
        TRIANGLE,
        ROTATION_NONE
    };


    // Rotation axis CheckBox
    static const char *xAxisChBxText = "X-Axis";
    static Vector2 xAxisChBxTextDimension = MeasureTextEx(font, xAxisChBxText, fontSize, fontSpacing);
    static float xAxisXOffset = 10.0f;

    static CheckBox xAxisChBx = {
        {width - xAxisChBxTextDimension.x - 2 * chBxInitX - chBxWidth * 0.5f, chBxInitY, chBxWidth, chBxHeight},
        {xAxisChBxText, (int)(width - xAxisChBxTextDimension.x - 2 * xAxisXOffset), chBxTextInitY, fontSize, RAYWHITE},
        WHITE,
        false,
        SHAPE_NONE,
        X_AXIS
    };

    static const char *yAxisChBxText = "Y-Axis";
    static Vector2 yAxisChBxTextDimension = MeasureTextEx(font, yAxisChBxText, fontSize, fontSpacing);

    static CheckBox yAxisChBx = {
        {width - xAxisChBxTextDimension.x - 2 * chBxInitX - chBxWidth * 0.5f, chBxInitY + chBxYoffset, chBxWidth, chBxHeight},
        {yAxisChBxText, (int)(width - yAxisChBxTextDimension.x - 2 * xAxisXOffset), chBxTextInitY + chBxTextYOffset, fontSize, RAYWHITE},
        RED,
        true,
        SHAPE_NONE,
        Y_AXIS
    };

    static const char *zAxisChBxText = "Z-Axis";
    static Vector2 zAxisChBxTextDimension = MeasureTextEx(font, zAxisChBxText, fontSize, fontSpacing);

    static CheckBox zAxisChBx = {
        {width - xAxisChBxTextDimension.x - 2 * chBxInitX - chBxWidth * 0.5f, chBxInitY + 2.0f * chBxYoffset, chBxWidth, chBxHeight},
        {zAxisChBxText, (int)(width - zAxisChBxTextDimension.x - 2 * xAxisXOffset), chBxTextInitY + 2 * chBxTextYOffset, fontSize, RAYWHITE},
        WHITE,
        false,
        SHAPE_NONE,
        Z_AXIS
    };


    static CheckBox checkBoxes[] = {circleChBx, ringChBx, triangleChBx};
    static unsigned int checkBoxesLength = sizeof(checkBoxes) / sizeof(CheckBox);

    static CheckBox checkBoxesRot[] = {xAxisChBx, yAxisChBx, zAxisChBx};
    static unsigned int checkBoxesRotLength = sizeof(checkBoxesRot) / sizeof(CheckBox);
    //! MENU INIT

    angle += 3 * PI * GetFrameTime();
    BeginDrawing();
    ClearBackground(backgroundColor);
    //! MENU
    UserInputCheckBox(checkBoxes, checkBoxesLength);
    UserInputCheckBox(checkBoxesRot, checkBoxesRotLength);

    Shape selectedShape;
    Rotation selectedRot;

    for (int i = 0; i < checkBoxesLength || i < checkBoxesRotLength; i++)
    {
        if(i < checkBoxesLength && checkBoxes[i].chBxselected)
        {
            selectedShape = checkBoxes[i].chBxshape;
        }

        if(i < checkBoxesRotLength && checkBoxesRot[i].chBxselected)
        {
            selectedRot = checkBoxesRot[i].chBxRot;
        }
    }

    DrawTextEx(font, "Select Filling Shape Of Cube : ", {chBxInitX, chBxInitY * 0.40f}, fontSize, fontSpacing, WHITE);
    DrawCheckBox(checkBoxes, checkBoxesLength);

    // Rotation axis text
    const char *rotHeading = "Select Rotational Axis : ";
    Vector2 rotHeadingWidth = MeasureTextEx(font, rotHeading, fontSize, fontSpacing);
    DrawTextEx(font, rotHeading, {width - rotHeadingWidth.x - 10.0f, chBxInitY * 0.40f}, fontSize, fontSpacing, WHITE);
    DrawCheckBox(checkBoxesRot, checkBoxesRotLength);
    //! MENU

    for (int i = 0; i < GRID_COUNT * GRID_COUNT * GRID_COUNT; i++)
    {
        // translate to origin - for rotation
        float xPrime = cube.points[i].x - xp;
        float yPrime = cube.points[i].y - yp;
        float zPrime = cube.points[i].z - zp;

        float newX = 0.0f;
        float newY = 0.0f;
        float newZ = 0.0f;

        switch (selectedRot)
        {
            case X_AXIS:
            {
                // performing rotation x-axis - for rotation
                newX = xPrime;
                newY = yPrime * cosf(DEG2RAD * angle) - zPrime * sinf(DEG2RAD * angle);
                newZ = yPrime * sinf(DEG2RAD * angle) + zPrime * cosf(DEG2RAD * angle);
                break;
            }

            case Y_AXIS:
            {
                // performing rotation y-axis - for rotation
                newX = xPrime * cosf(DEG2RAD * angle) + zPrime * sinf(DEG2RAD * angle);
                newY = yPrime;
                newZ = -xPrime * sinf(DEG2RAD * angle) + zPrime * cosf(DEG2RAD * angle);
                break;
            }

            case Z_AXIS:
            {
                // performing rotation z-axis - for rotation
                newX = xPrime * cosf(DEG2RAD * angle) - yPrime * sinf(DEG2RAD * angle);
                newY = xPrime * sinf(DEG2RAD * angle) + yPrime * cosf(DEG2RAD * angle);
                newZ = zPrime;
                break;
            }

            default:
            {
                break;
            }
        }

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
                DrawTextEx(font, defaultText, {width*0.5f - defaultTextWidth * 0.5f,height*0.5f},fontSize, fontSpacing ,WHITE);
                break;
            }
        }
    }
    EndDrawing();
}


void DrawCheckBox(CheckBox *checkBoxes, int size)
{
    float roundness = 0.5f;
    float segments = 10.0f;
    for (int i = 0; i < size; i++)
    {
        Rectangle rect1 = checkBoxes[i].chBxrect;
        DrawRectangleRounded(rect1, roundness, segments, checkBoxes[i].chBxcolor);
        DrawTextEx(font ,checkBoxes[i].text.str, {(float)checkBoxes[i].text.posX, (float)checkBoxes[i].text.posY}, checkBoxes[i].text.fontSize, fontSpacing,checkBoxes[i].text.textColor);
    }
}


void UserInputCheckBox(CheckBox *checkBoxes, int size)
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