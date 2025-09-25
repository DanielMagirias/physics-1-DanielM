/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"
#include <string>
#include <vector>

const unsigned int TARGET_FPS = 50;// frames/second
float dt = 1.0f / TARGET_FPS; // seconds/frame
float time = 0;


class PhysicsObject {

public:
    Vector2 position = { 500, 500 };
    Vector2 velocity = { 0, 0 };
    float mass = 1; // in kg
    float radius = 15; // circle radius in pixels
    std::string name = "";

    void draw() {
        DrawCircle(position.x, position.y, radius, RED);
        //DrawText(name, position.x, position.y, 15, Color {0, 0, 0, 255});
    }
};


class PhysicsWorld {

public:
    Vector2 accelerationGravity = { 0, 98 }; // change Y value for more gravity
    std::vector<PhysicsObject> objects;

    void add(PhysicsObject newObj) { // add to the simulation
        objects.push_back(newObj);
    }

    void update() {
        for (int i = 0; i < objects.size(); i++) {

            //Velocity = change in position / time, therefore change in position = velocity * time
            objects[i].position += objects[i].velocity * dt;

            // acceleration = deltaV / time (change in velocity over time) therefore delvaV = acceleration * time
            objects[i].velocity = objects[i].velocity + accelerationGravity * dt;

        }

    }
};


//Lab 1 stuff
float speed = 100;
float angle = 0;
Vector2 launchPos{ 100, 700 };


//Lab 2 stuff
//PhysicsObject obj;
PhysicsWorld world;


void update() 
{
    dt = 1.0f / TARGET_FPS;
    time += dt;

    world.update();


    if (IsKeyPressed(KEY_SPACE)) {

        PhysicsObject newBird;
        newBird.position = launchPos;
        newBird.velocity = { speed * (float)cos(angle * DEG2RAD), -speed * (float)sin(angle * DEG2RAD) };
        //newBird.radius = (rand() % 26) + 5; random circle size

        world.add(newBird); //add bird to simulation
    }

    //if (IsKeyPressed(KEY_C)) {
    //    for each(PhysicsObject newBird in world.objects)
    //    {
    //        world.objects.erase (world.objects.begin() + world.objects.count());
    //    }
    //}
    
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



            GuiSliderBar(Rectangle{ 55, 90, 800, 25 }, "Gravity Y", TextFormat("Gravity Y: %.0f Px/sec^2", world.accelerationGravity.y), &world.accelerationGravity.y, -180, 180); // Lab 2

            for (int i = 0; i < world.objects.size(); i++) {
                world.objects[i].draw();
            }


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
