/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"

const unsigned int TARGET_FPS = 50;// frames/second
float dt = 1.0f / TARGET_FPS; // seconds/frame
float time = 0;

//float x = 500;
//float y = 500;
//float frequency = 1;
//float amplitude = 100; All Variables for the circles

//test


void update() 
{
    dt = 1.0f / TARGET_FPS;
    time += dt;

    //x = x + (-sin(time * frequency)) * frequency * amplitude * dt;
    //y = y + (cos(time * frequency)) * frequency * amplitude * dt;
}

void draw() 
{
        BeginDrawing();
            ClearBackground(WHITE);

            GuiSliderBar(Rectangle{ 60, 50, 1000, 50 }, "Time", TextFormat("%.2f", time), &time, 0, 240);
            DrawText(TextFormat("Time: %.2f", time), GetScreenWidth() / 2 - 75, 10, 30, LIGHTGRAY);

            //DrawCircle(x, y, 70, RED);
            //DrawCircle(500 + cos(time * frequency)* amplitude, 500 + sin(time * frequency) * amplitude, 70, GREEN); For the Moving Circles

            //DrawLine(0, 700, 1200, 700, GREEN); //Ground (line version)
            DrawRectangle(0, 700, 1200, 100, GREEN); // Ground (Rectangel version)

            DrawText("Daniel Magirias 101552396", 10, GetScreenHeight() - 30, 20, BLACK);

        EndDrawing();
}
int main()
{
    // InitialWidth and InitialHeight are from including "game.h"   
    InitWindow(InitialWidth, InitialHeight, "GAME2005 Daniel Magirias 101552396");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    CloseWindow();
    return 0;
}
