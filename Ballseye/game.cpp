#include "raylib.h"
#include <cstdio>

struct Ball
{
    Vector2 position;
    Vector2 speed;
    float radius;
    Color color;
    bool active;
};


enum class GameState
{
    MENU,
    GAMEON,
    PAUSE,
    GAMEOVER
};

bool lastFramePause = false;

int main()
{
    // Initialization

    int windowDimensions[2];
    windowDimensions[0] = 1000;
    windowDimensions[1] = 800;

    InitWindow(windowDimensions[0], windowDimensions[1], "Ballseye");
    SetTargetFPS(60);

    Texture2D backTexture = LoadTexture("textures/background.png");

    const int gravity{800}; // acceleration due to gravity (pixels/sec/sec)
    const int amountofballs{50};
    int popercount{0}; // number of balls popped
    int credits{0};
    int timer = 15; // timer in seconds
    int frameCount = 0;
    bool displayCredits = false;

    Ball ball[amountofballs]{};

    GameState gameState = GameState::MENU;


    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(backTexture, 0, 0, WHITE);
            
        if (gameState == GameState::MENU)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                credits++;
            }
            if (IsKeyPressed(KEY_SPACE) && credits > 0)
            {
                gameState = GameState::GAMEON;
                for(int i = 0; i < amountofballs; i++)
                {   
                    ball[i].position.x = GetRandomValue(10, windowDimensions[0]);
                    ball[i].position.y = GetRandomValue(10, windowDimensions[1]);
                    ball[i].speed.x = GetRandomValue(-100, 300);
                    ball[i].speed.y = GetRandomValue(-100, 300);
                    ball[i].radius = GetRandomValue(10, 50);
                    ball[i].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
                    ball[i].active = true;
                }
            }
			const char *title = "BallsEye";
			MeasureText(title, 300);
			int titleHeight = 50;
			int titleX = windowDimensions[0] / 2 + 380 - 500;
			int titleY = windowDimensions[1] / 2 - titleHeight / 2;

			DrawText(title, titleX, titleY, 50, GREEN);

			const char *msg = "PRESS ENTER TO INSERT CREDITS AND SPACE TO PLAY";
			int msgWidth = MeasureText(msg, 10);
			int msgHeight = 30;
			int msgX = windowDimensions[0] - 640 - msgWidth;
			int msgY = windowDimensions[1] / 2 - msgHeight / 2 + 250;

			DrawText(msg, msgX, msgY, 30, RAYWHITE);
			DrawText("USE THE MOUSE TO POP ALL THE BALLS!", 150, 235, 35, WHITE);
			if (credits == 0)
			{
				DrawText("OUT OF CREDITS", 370, 700, 30, RED);
			}
		}
	if(gameState == GameState::GAMEON || gameState == GameState::PAUSE)
	{
		if (IsKeyPressed(KEY_SPACE) && !lastFramePause)
		{
			if (gameState == GameState::GAMEON)
			{
				gameState = GameState::PAUSE; // Pause the game
			}
			else if (gameState == GameState::PAUSE)
			{	
				gameState = GameState::GAMEON;  // Resume the game
			}
			lastFramePause = true;
		}
		else if (!IsKeyPressed(KEY_SPACE))
		{
			lastFramePause = false;
		}
		if (gameState == GameState::PAUSE)
		{
			DrawText("Hit SPACE when READY!", 220, 100, 45, WHITE);
		}
	}
	if (gameState == GameState::GAMEON) 
	{
		if (timer > 0 && popercount < amountofballs) 
		{
			frameCount++;
		}
		if (frameCount / 60 >= 1)
		{
			timer--;
			frameCount = 0;
		}
		if (timer <= 0 || popercount >= amountofballs)
		{
			gameState = GameState::GAMEOVER;
			ball[amountofballs].active = false;
		}
	}

	if (gameState == GameState::GAMEOVER)
	{
		static bool decreaseCredits = false; 
		if (timer > 0 && amountofballs <= popercount)
		{
			gameState = GameState::GAMEOVER;
			DrawText("GAME OVER", 200, 300, 100, RED);
			DrawText("YOU WIN", 260, 450, 100, GREEN);
		}

		else if (timer <= 0 && amountofballs > popercount)
		{
			gameState = GameState::GAMEOVER;
			DrawText("GAME OVER", 200, 300, 100, RED);
			DrawText("YOU LOSE", 235, 450, 100, RED);
		}
		
		for(int i = 0; i < amountofballs; i++)
		{
			ball[i].active = false;
		}
		if(!decreaseCredits) // only decrease credits once
		{
			if (credits > 0)
			{
				credits --;
			}
			decreaseCredits = true;
		}

		DrawText("PRESS ENTER TO PLAY AGAIN", 280, 700, 30, WHITE);
		DrawText("Press TAB to retrieve credits ", 280, 750, 30, WHITE);
		if (IsKeyPressed(KEY_ENTER))
		{
			gameState = GameState::MENU;
			popercount = 0;
			timer = 15;
		}
		if (IsKeyPressed(KEY_TAB)) // retrieve credits
		{
			displayCredits = true;
		}
		if(displayCredits)
		{
			char creditRem[32];
			sprintf(creditRem, "CREDITS RETRIVED! Press ESC to Exit");
			MeasureText(creditRem, 30);
			DrawText(creditRem, 190, 600, 30, GREEN);
			credits = 0;
		}

	}
	Vector2 mousePosition = GetMousePosition(); 
		if(gameState == GameState::GAMEON) // prevents popping balls in paused state
		{	
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) // pop balls
			{
				for (int i = 0; i < amountofballs; i++)
				{
					if (ball[i].active && CheckCollisionPointCircle(mousePosition, ball[i].position, ball[i].radius))
					{
						ball[i].speed.x -= 5;
						ball[i].speed.y *= 0.5;
						ball[i].active = false;
						popercount++;
					}
				}
			} 
		}
        // credits / scores / time
        char countText[32];
        sprintf(countText, "Popped: %i", popercount);
        int TextWidth = MeasureText(countText, 30);
        DrawText(countText, GetScreenWidth() - TextWidth - 10, 10, 30, WHITE);

        char creditText[32];
        sprintf(creditText, "Credits: %i", credits);
        int creditWidth = MeasureText(creditText, 30);
        DrawText(creditText, GetScreenWidth() - creditWidth - 10, 50, 30, WHITE);

        char screentimer[32];
        sprintf(screentimer, "Time: %i", timer);
        int timerWidth = MeasureText(screentimer, 30);
        DrawText(screentimer, GetScreenWidth() - timerWidth - 10 , 90 , 30,   WHITE);

        // Draw balls
        for (int i = 0; i < amountofballs; i++)
        {
            if(ball[i].active)
            {
                ball[i].position.x += ball[i].speed.x * GetFrameTime();
                ball[i].position.y += ball[i].speed.y * GetFrameTime();
                ball[i].speed.y += gravity * GetFrameTime();

                if (ball[i].position.x >= windowDimensions[0] - ball[i].radius) // bounce off right wall
                {
                    ball[i].speed.x *= -1;
                }
                if (ball[i].position.x <= ball[i].radius) // bounce off left wall
                {
                    ball[i].speed.x *= -1;
                    ball[i].position.x = ball[i].radius;
                }

                if (ball[i].position.y >= windowDimensions[1] - ball[i].radius) // bounce off floor
                {
                    ball[i].speed.y *= -0.95;
                    ball[i].position.y = windowDimensions[1] - ball[i].radius;
                }
                else if (ball[i].position.y - ball[i].radius <= 0)  // bounce off ceiling
                {
                    ball[i].speed.y *= -1;
                    ball[i].position.y = ball[i].radius;
                }
                DrawCircleV(ball[i].position, ball[i].radius, ball[i].color); // draw ball
        
            }
        }

    EndDrawing();
    }
    UnloadTexture(backTexture); 
    WindowShouldClose();  
    return 0;
}   