#include "raylib.h"

int main()
{

    //window dimensions
    int winwidth = 800;
    int winheight = 450;
    

    //initialize window
    InitWindow(winwidth, winheight, "Axegame");
    //circle coordinates
    int circle_x{200};
    int circle_y{200};

    //axe coordinates
    int axe_x{400};
    int axe_y{0};
    int direction {10};

    //main game loop
    SetTargetFPS(60);
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(WHITE);

        //Game logic begins here
        DrawText("Welcome to Axegame", 80, 10, 20, WHITE);
        DrawCircle(circle_x, circle_y, 50, BLUE);
        DrawRectangle(axe_x, axe_y, 50, 50, RED);

        //move the axe
        
        axe_y += direction;
        if(axe_y > 450 || axe_y < 0)
        {
            direction = -direction;
        }

        if(IsKeyDown(KEY_D) && circle_x < 350) //establishes right boundaries for the circle
        {
            circle_x += 10;
        }
        if(IsKeyDown(KEY_A) && circle_x > 0) //establishes left boundaries for the circle
        {
            circle_x -= 10;
        }
        

        //Game logic ends here
        EndDrawing();
    }
}