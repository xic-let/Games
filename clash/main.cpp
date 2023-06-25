#include "raylib.h"
#include "raymath.h"

int main ()
{
    const int windowWidth {384};
    const int windowHeight {384};
    InitWindow(windowWidth, windowHeight, "Clash");

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 direction{};
        if (IsKeyDown(KEY_LEFT)) direction.x -= 1.0;
        if (IsKeyDown(KEY_RIGHT)) direction.x += 1.0;
        if (IsKeyDown(KEY_UP)) direction.y -= 1.0;
        if (IsKeyDown(KEY_DOWN)) direction.y += 1.0;
        if (Vector2Length(direction) != 0.0)
        {
            mapPos = Vector2Subtract(mapPos, Vector2Normalize(direction));
        }
        

        DrawTextureEx(map, mapPos, 0.0, 4.0, WHITE);
        
        EndDrawing();
    }
    CloseWindow();
}