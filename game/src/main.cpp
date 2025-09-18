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

//Lab 1 stuff
float speed = 100;
float angle = 0;
Vector2 launchPos{ 100, 700 };


//Lab 2 stuff
Vector2 position = { 500, 500 };
Vector2 velocity = { 0, 0 };
Vector2 accelerationGravity = { 0, 98 }; // change Y value for more gravity

void update() 
{
    dt = 1.0f / TARGET_FPS;
    time += dt;

    //Velocity = change in position / time, therefore change in position = velocity * time
    position += velocity * dt;

    // acceleration = deltaV / time (change in velocity over time) therefore delvaV = acceleration * time
    velocity = velocity + accelerationGravity * dt;


    if (IsKeyPressed(KEY_SPACE)) {

        position = launchPos;
        velocity = { speed * (float)cos(angle * DEG2RAD), -speed * (float)sin(angle * DEG2RAD) };
    }
    
}

void draw() 
{
        BeginDrawing();

            ClearBackground(WHITE);


            GuiSliderBar(Rectangle{ 55, 0, 800, 25 }, "Time", TextFormat("%.2f", time), &time, 0, 240);
            //DrawText(TextFormat("Time: %.2f", time), GetScreenWidth() / 2 - 75, 10, 30, LIGHTGRAY); //Time text above the slider bar
            GuiSliderBar(Rectangle{ 55, 30, 800, 25 }, "Speed", TextFormat("Speed: %.0f", speed), &speed, -1000, 1000); // Lab 1
            GuiSliderBar(Rectangle{ 55, 60, 800, 25 }, "Angle", TextFormat("Angle: %.0f Degrees", angle), &angle, -180, 180); // Lab 1


            Vector2 velocity = { speed * cos(angle * DEG2RAD), -speed * sin(angle * DEG2RAD) }; // DEG2RAD is PI / 180.0
            GuiSliderBar(Rectangle{ 55, 120, 800, 25 }, "Launch X", TextFormat("Launch Position X: %.0f units", launchPos.x), &launchPos.x, GetScreenWidth() - (GetScreenWidth() - 1), GetScreenWidth() - 1);
            GuiSliderBar(Rectangle{ 55, 150, 800, 25 }, "Launch Y", TextFormat("Launch Position Y: %.0f units", launchPos.y), &launchPos.y, GetScreenHeight() - (GetScreenHeight() - 1), GetScreenHeight() - 1);
            DrawLineEx(launchPos, launchPos + velocity, 3, RED);



            GuiSliderBar(Rectangle{ 55, 90, 800, 25 }, "Gravity Y", TextFormat("Gravity Y: %.0f Px/sec^2", accelerationGravity.y), &accelerationGravity.y, -180, 180); // Lab 2
            DrawCircle(position.x, position.y, 15, RED);


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
