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

enum PhysicsShape {
    CIRCLE,
    HALF_SPACE
};


class PhysicsObject {

public:

    bool isStatic = false; //if this is true = object doesnt move from gravity
    Vector2 position = { 500, 500 };
    Vector2 velocity = { 0, 0 };
    float mass = 1; // in kg
    //float radius = 15; // circle radius in pixels
   std::string name = "";
   Color color = BLUE;

    virtual void draw() {
        DrawCircle(position.x, position.y, 2, color); // draws like a 2 pixel dot
        //draw name maybe
    }

    virtual PhysicsShape Shape() = 0;
};

class PhysicsBox : public PhysicsObject {

public:

    Vector2 size{ 50, 50 }; // x = width, y = height


    void draw() override {
        DrawRectangle(position.x, position.y, size.x, size.y, BLUE);
        //draw name
    }


    // Inherited via PhysicsObject
    PhysicsShape Shape() override
    {
        return PhysicsShape();
    }

};

class PhysicsCircle : public PhysicsObject {

public:

    float radius = 15; // circle radius in pixels


    void draw() override {

        DrawCircle(position.x, position.y, radius, color);
        //DrawText(name, position.x, position.y, 15, Color {0, 0, 0, 255});
    }

    PhysicsShape Shape() override
    {
        return CIRCLE;
    }
};

class PhysicsHalfSpace : public PhysicsObject {

private:
    float rotation = 0;
    Vector2 normal = { 0, -1 };

public:

    void setRotationDegrees(float rotationInDegrees) {

        rotation = rotationInDegrees;
        normal = Vector2Rotate({ 0, -1 }, rotation * DEG2RAD);
    }

    float getRotation() {

        return rotation;
    }

    Vector2 getNormal() {

        return normal;
    }
    

    void draw() override {

        DrawCircle(position.x, position.y, 8, color);
        DrawLineEx(position, position + normal * 30, 1, color);
        
        Vector2 parallelToSurface = Vector2Rotate(normal, PI / 2.0f);
        DrawLineEx(position - parallelToSurface * 4000, position + parallelToSurface * 4000, 1, color);

    }

    // Inherited via PhysicsObject
    PhysicsShape Shape() override
    {
        return HALF_SPACE;
    }
};

bool CircleCircleOverlap(PhysicsCircle* circleA, PhysicsCircle* circleB) {


    Vector2 Displacement = circleB->position - circleA->position;
    float distance = Vector2Length(Displacement);
    float sumOfRadii = circleA->radius + circleB->radius;

    if (sumOfRadii > distance) {
        return true; // overlapping
    }

    else {
        return false; // not overlapping
    }
}

bool CircleHalfspaceOverlap(PhysicsCircle* circle, PhysicsHalfSpace* halfspace) {

    Vector2 displacementToCircle = circle->position - halfspace->position;

    float distanceAlongNormal = Vector2DotProduct(displacementToCircle, halfspace->getNormal());

    Vector2 projectionDisplacementOntoNormal = halfspace->getNormal() * distanceAlongNormal;
    DrawLineEx(circle->position, circle->position - projectionDisplacementOntoNormal, 1, GRAY);
    Vector2 midpoint = circle->position - projectionDisplacementOntoNormal * 0.5f;
    DrawText(TextFormat("D: %6.0f", distanceAlongNormal), midpoint.x, midpoint.y, 30, GRAY);

    return distanceAlongNormal < circle->radius;
}



class PhysicsWorld {

public:

    Vector2 accelerationGravity = { 0, 98 }; // change Y value for more gravity
    std::vector<PhysicsObject*> objects;

    void add(PhysicsObject* newObj) { // add to the simulation

        objects.push_back(newObj);
    }

    void update() {

        for (int i = 0; i < objects.size(); i++) {

            PhysicsObject* object = objects[i];
            
            if (object->isStatic) {
                continue;
             }

            //Velocity = change in position / time, therefore change in position = velocity * time
            objects[i]->position += objects[i]->velocity * dt;

            // acceleration = deltaV / time (change in velocity over time) therefore delvaV = acceleration * time
            objects[i]->velocity = objects[i]->velocity + accelerationGravity * dt;

        }

        checkCollisions();

    }

    void checkCollisions() {
        

        for (int i = 0; i < objects.size(); i++) {
            objects[i]->color = GREEN;
        }

        for (int i = 0; i < objects.size(); i++) {

            for (int j = 0; j < objects.size(); j++) {

                if (i == j) continue;

                PhysicsObject* objPointerA = objects[i];
                PhysicsObject* objPointerB = objects[j];

                PhysicsShape shapeOfA = objPointerA->Shape();
                PhysicsShape shapeOfB = objPointerB->Shape();

                bool didOverlap = false;

                //PhysicsCircle* circlePointer = (PhysicsCircle*)objPointerA;
                //PhysicsCircle* circlePointerB = (PhysicsCircle*)objPointerB;

                if (shapeOfA == CIRCLE && shapeOfB == CIRCLE)
                {

                    didOverlap = CircleCircleOverlap((PhysicsCircle*)objPointerA, (PhysicsCircle*)objPointerB);

                    /*if (CircleCircleOverlap((PhysicsCircle*)objPointerA, (PhysicsCircle*)objPointerB)) {

                        objPointerA->color = RED;
                        objPointerB->color = RED;
                    }

                    else {
                        objPointerA->color = BLUE;
                        objPointerB->color = BLUE;
                    }*/

                }

                else if (shapeOfA == CIRCLE && shapeOfB == HALF_SPACE) {

                   didOverlap = CircleHalfspaceOverlap((PhysicsCircle*)objPointerA, (PhysicsHalfSpace*)objPointerB);
                }

                else if (shapeOfA == HALF_SPACE && shapeOfB == CIRCLE) {

                    didOverlap = CircleHalfspaceOverlap((PhysicsCircle*)objPointerB, (PhysicsHalfSpace*)objPointerA);
                }

                if (didOverlap) {
                    objPointerA->color = RED;
                    objPointerB->color = RED;
                }
               

            }

            
        }

        //CircleCircleOverlap()
        
    }
};


//Lab 1 stuffpoint
float speed = 100;
float angle = 0;
Vector2 launchPos{ 100, 700 }; // for the gui slider that controls x and y positions for launch position.


//Lab 2 stuff
PhysicsWorld world;
PhysicsHalfSpace halfspace;


void clearWorld() {

    for (int i = world.objects.size() - 1; i >= 0; i--) {

        if (world.objects[i]->isStatic) { // so the halfspace doesnt get deleted
            continue;
        }

        world.objects.erase(world.objects.begin() + i);
    }

}


void update() 
{
    dt = 1.0f / TARGET_FPS;
    time += dt;

    world.update();


    if (IsKeyPressed(KEY_SPACE)) {

        PhysicsCircle* newBird = new PhysicsCircle();
        newBird->position = launchPos;
        newBird->velocity = { speed * (float)cos(angle * DEG2RAD), -speed * (float)sin(angle * DEG2RAD) };
        //newBird.radius = (rand() % 26) + 5; random circle size

        world.add(newBird); //add bird to simulation
        
    }

    if (IsKeyPressed(KEY_C)) {

        clearWorld();
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


            GuiSliderBar(Rectangle{ 55, 90, 800, 25 }, "Gravity Y", TextFormat("Gravity Y: %.0f Px/sec^2", world.accelerationGravity.y), &world.accelerationGravity.y, -180, 180); // Lab 2

            for (int i = 0; i < world.objects.size(); i++) {
                world.objects[i]->draw();
            }


            GuiSliderBar(Rectangle{ 1100, 0, 400, 25 }, "HalfspaceX", TextFormat("Speed: %.0f", halfspace.position.x), &halfspace.position.x, 0, GetScreenWidth());
            GuiSliderBar(Rectangle{ 1100, 30, 400, 25 }, "HalfspaceY", TextFormat("Speed: %.0f", halfspace.position.y), &halfspace.position.y, 0, GetScreenHeight());

            float halfspaceRotation = halfspace.getRotation();
            GuiSliderBar(Rectangle{ 1100, 60, 400, 25 }, "Halfspace Rotation", TextFormat("%.0f", halfspace.getRotation()), &halfspaceRotation, -360, 360);
            halfspace.setRotationDegrees(halfspaceRotation);



            DrawText(TextFormat("Active Obj: %d", world.objects.size()), 1580, 10, 15, BLACK);



            DrawText("Daniel Magirias 101552396", 10, GetScreenHeight() - 30, 20, BLACK);

        EndDrawing();
}


int main()
{
    // InitialWidth and InitialHeight are from including "game.h"   
    InitWindow(InitialWidth, InitialHeight, "GAME2005 Daniel Magirias 101552396");
    SetTargetFPS(TARGET_FPS);

    halfspace.position = { 500, 700 };
    halfspace.isStatic = true;
    world.add(&halfspace);

    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    CloseWindow();
    return 0;
}
