#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isonGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0f;
        //update animation frame
        data.rec.x = data.frame * data.rec.width; 
        data.frame++;
        if (data.frame > maxFrame ) //maxFrame is the last frame of the animation
        {
            data.frame = 0;
        }
    }
    return data;
}


int main ()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher");

    
    const int gratity{1'000}; //acceleration due to gravity (pixels/sec/sec)
   
 
    //nebula variables
    Texture2D nebulas = LoadTexture("textures/12_nebula_spritesheet.png");  // Texture2D type, same as image.type, used for texture storage
    
    const int sizeOfNebulae{10};

    AnimData nebulae[sizeOfNebulae]{};//array of nebulae //create a for loop to make the code cleaner and prevent repetition

    for(int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0f; // Set rectangle x position to 0
        nebulae[i].rec.y = 0.0f; // Set rectangle y position to 0
        nebulae[i].rec.width = nebulas.width/8; // Set rectangle width to 1/8th of nebula width
        nebulae[i].rec.height = nebulas.height/8; // Set rectangle height to nebula height
        nebulae[i].pos.y = windowDimensions[1] - nebulas.height/8; // Set nebula to ground of the screen
        nebulae[i].frame = 0; // Set animation frame to 0
        nebulae[i].updateTime = 0; // amount of time between each frame
        nebulae[i].runningTime = 1.0/16.0f; // time since last frame
        nebulae[i].pos.x = windowDimensions[0] + i * 300 ; //nebula position (pixels)
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x}; //finish line position (pixels)

    //nebula velocity
    int nebVel{-200}; //speed of nebula (pixels/sec)


    //scarfy variables - another way to initialize a scruct
    Texture2D scarfy = LoadTexture("textures/scarfy.png");  // Texture2D type, same as image.type, used for texture storage
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6; // Set rectangle width to 1/6th of scarfy width
    scarfyData.rec.height = scarfy.height; // Set rectangle height to scarfy height
    scarfyData.rec.x = 0; // Set rectangle x position to 0
    scarfyData.rec.y = 0; // Set rectangle y position to 0
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2; // Set scarfy to center of the screen
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height; // Set scarfy to ground of the screen
    scarfyData.frame = 0; // Set animation frame to 0
    scarfyData.updateTime = 1.0f/12.0f; // amount of time between each frame
    scarfyData.runningTime = 0.0f; // time since last frame
    
    int velocity{0};
    bool isinAir;
    const int jumpSpeed{-600}; //speed of jump (pixels/sec)

    Texture2D background = LoadTexture("textures/far-buildings.png");  // Texture2D type, same as image.type, used for texture storage
    float bgx{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgx{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgx{};

    bool isColliding{};
   
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
            const float dt{GetFrameTime()};  // Delta time, used for frame independent movement - time since last frame
            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
    
                ClearBackground(WHITE);

                bgx -= 20 * dt; //UPDATE background position
                if (bgx <= -background.width *2)
                {
                    bgx = 0.0;
                }
                mgx -= 40 * dt; //UPDATE midground position
                if (mgx <= -midground.width *2)
                {
                    mgx = 0.0;
                }
                fgx -= 80 * dt; //UPDATE foreground position
                if (fgx <= -foreground.width *2)
                {
                    fgx = 0.0;
                }

                //draw Background
                Vector2 bg1Pos{bgx,0.0};
                DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
                Vector2 bg2Pos{bgx + background.width * 2.0f, 0.0};
                DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

                //draw Midground
                Vector2 mg1Pos{mgx,0.0};
                DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
                Vector2 mg2Pos{mgx + midground.width * 2.0f, 0.0};
                DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

                //draw Foreground
                Vector2 fg1Pos{fgx,0.0};
                DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
                Vector2 fg2Pos{fgx + foreground.width * 2.0f, 0.0};
                DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

                if(isonGround(scarfyData, windowDimensions[1])) //CHECK FOR GROUND
                {
                    velocity = 0;
                    isinAir = false;
                }  
                else
                {
                    velocity += gratity * dt;
                    isinAir = true;
                    
                }
                if(IsKeyPressed(KEY_SPACE) && !isinAir) //CHECK FOR JUMP
                { 
                    velocity += jumpSpeed; 
                }
                
                for(int i = 0; i < sizeOfNebulae; i++) //CHECK FOR NEBULA COLLISION
                {
                   nebulae[i].pos.x += nebVel * dt; //UPDATE each nebula POSITION
                }

                finishLine += nebVel * dt;

                scarfyData.pos.y += velocity * dt; //UPDATE scarfy POSITION
                
                if(!isinAir) //stop walking animation when in air & update scarfy update animation frame
                {
                    scarfyData = updateAnimData(scarfyData, dt, 5);
                }
                for(int i= 0; i< sizeOfNebulae; i++) //UPDATE nebulae animation frame
                {
                    nebulae[i] = updateAnimData(nebulae[i], dt, 7);
                }
                
                for(AnimData nebula : nebulae) //CHECK FOR COLLISION
                {
                    float pad{50};
                    Rectangle nevrec{
                        nebula.pos.x + pad,
                        nebula.pos.y + pad,
                        nebula.rec.width-2*pad,
                        nebula.rec.height-2*pad
                    };
                    Rectangle scarfyrec{
                        scarfyData.pos.x + pad,
                        scarfyData.pos.y + pad,
                        scarfyData.rec.width-2*pad,
                        scarfyData.rec.height-2*pad
                    };
                    if (CheckCollisionRecs(nevrec, scarfyrec))
                    {
                        isColliding = true;
                        
                    }
                }
                
                    if(isColliding)
                    {
                        DrawText("Game Over", windowDimensions[0]/4,windowDimensions[1]/2, 40, RED);
                    }
                    else if(scarfyData.pos.x >= finishLine)
                    {
                        DrawText("You Win!", windowDimensions[0]/4,windowDimensions[1]/2, 40, GREEN);
                    }
                    else
                    {
                        for(int i = 0; i < sizeOfNebulae; i++)
                        {
                            DrawTextureRec(nebulas, nebulae[i].rec, nebulae[i].pos, WHITE);
                                    //draw nebula
                        }    
                        // draw scarfy
                        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE); 
                    }

            EndDrawing();
            //----------------------------------------------------------------------------------
    }
    UnloadTexture(scarfy);      // Unload texture from VRA
    UnloadTexture(nebulas);      // Unload texture from VRA
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();        // Close window and OpenGL context 
}