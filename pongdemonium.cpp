/*****************************************************************************************************
*
*   Pongdemonium: a variation of the classic game
*
*   Game created using raylib v4.2 - A simple and easy-to-use library to enjoy videogames programming
*   See www.raylib.com and include/raylib.h for more details
*
******************************************************************************************************/

#include "include/raylib.h"

//----------------------------------------------------------------------------------------------------
// Definition of constants
//----------------------------------------------------------------------------------------------------
const int screenWidth = 1000;
const int screenHeight = 800;

//----------------------------------------------------------------------------------------------------
// Definition of custom types
//----------------------------------------------------------------------------------------------------
struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool visible;

    void Draw(Color colour)
    {
        DrawCircle(position.x, position.y, radius, colour);     // Draw a color-filled circle
    }

    void Reset()
    {
        position.x = screenWidth / 2;
        position.y = screenHeight / 2;
        velocity.x = 400;
        velocity.y = 400;
    }
};

struct Player {
    Vector2 position;
    Vector2 size;
    int speed;

    void Draw(Color colour)
    {
        DrawRectangleRec(GetRectangle(), colour);       // Draw a color-filled rectangle
    }

    Rectangle GetRectangle()
    {
        return Rectangle{ position.x - (size.x / 2), position.y - (size.y / 2), size.x, size.y };
    }
};

//----------------------------------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------------------------------
Player player1Left, player2Right;
Ball ball1, ball2;
int player1LeftScore, player2RightScore, frameCounter;
bool gamePaused = false;
bool gameWon = false;

//----------------------------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------------------------
void InitialiseGameObjects()
{
    // Initialise variables of player 1
    player1Left.position.x = 25;
    player1Left.position.y = screenHeight / 2;
    player1Left.size.x = 15;
    player1Left.size.y = 150;
    player1Left.speed = 1000;

    // Initialise variables of player 2
    player2Right.position.x = screenWidth - 25;
    player2Right.position.y = screenHeight / 2;
    player2Right.size.x = 15;
    player2Right.size.y = 150;
    player2Right.speed = 1000;
    
    // Initialise variables of ball 1
    ball1.position.x = screenWidth / 2;
    ball1.position.y = screenHeight/ 2;
    ball1.radius = 10;
    ball1.velocity.x = 400;
    ball1.velocity.y = 400;
    ball1.visible = true;

    // Initialise variables of ball 2
    ball2.position.x = screenWidth / 2;
    ball2.position.y = screenHeight/ 2;
    ball2.radius = 10;
    ball2.velocity.x = 400;
    ball2.velocity.y = 400;
    ball2.visible = false;

    // Initialise counters
    player1LeftScore = 0;
    player2RightScore = 0;
    frameCounter = 0;
}

//----------------------------------------------------------------------------------------------------
// Main entry point of program
//----------------------------------------------------------------------------------------------------
int main()
{
    // Initialisation
    //------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pongdemonium");

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    InitialiseGameObjects();

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        if (!gameWon)
        {
            // Update game state (one frame at a time)
            
            // Logic for position of game objects (sprites)
            //------------------------------------------------------------------------------------------------
            // Set bottom bound for player 1
            if (player1Left.position.y > screenHeight - (player1Left.size.y / 2))
            {
                player1Left.position.y = screenHeight - (player1Left.size.y / 2);
            }
            // Set top bound for player 1
            else if (player1Left.position.y < 0 + (player1Left.size.y / 2))
            {
                player1Left.position.y = 0 + (player1Left.size.y / 2);
            }

            // Set bottom bound for player 2
            if (player2Right.position.y > screenHeight - (player2Right.size.y / 2))
            {
                player2Right.position.y = screenHeight - (player2Right.size.y / 2);
            }
            // Set top bound for player 2
            else if (player2Right.position.y < 0 + (player2Right.size.y / 2))
            {
                player2Right.position.y = 0 + (player2Right.size.y / 2);
            }

            // Move ball 1 around the screen - change position by adding velocity in x and y directions
            ball1.position.x += ball1.velocity.x * GetFrameTime();      // Get time in seconds for last frame drawn (delta time) i.e. amount of time between frames
            ball1.position.y += ball1.velocity.y * GetFrameTime();      // to keep change of position (velocity) in sync with frame speed

            // Set bottom bound for ball 1
            if (ball1.position.y > screenHeight - ball1.radius)
            {
                ball1.position.y = screenHeight - ball1.radius;
                // Change the direction of the ball
                ball1.velocity.y *= -1;
            }
            // Set top bound for ball 1
            else if (ball1.position.y < 0 + ball1.radius)
            {
                ball1.position.y = 0 + ball1.radius;
                // Change the direction of the ball
                ball1.velocity.y *= -1;
            }

            if (ball2.visible)
            {
                // Move ball 2 around the screen - change position by adding velocity in x and y directions
                ball2.position.x += ball2.velocity.x * GetFrameTime();      // Get time in seconds for last frame drawn (delta time) i.e. amount of time between frames
                ball2.position.y += ball2.velocity.y * GetFrameTime();      // to keep change of position (velocity) in sync with frame speed

                // Set bottom bound for ball 1
                if (ball2.position.y > screenHeight - ball2.radius)
                {
                    ball2.position.y = screenHeight - ball2.radius;
                    // Change the direction of the ball
                    ball2.velocity.y *= -1;
                }
                // Set top bound for ball 1
                else if (ball2.position.y < 0 + ball2.radius)
                {
                    ball2.position.y = 0 + ball2.radius;
                    // Change the direction of the ball
                    ball2.velocity.y *= -1;
                }
            }

            // Logic for user input controls
            //------------------------------------------------------------------------------------------------
            // Set controls for player 1
            if (IsKeyDown(KEY_S))
            {
                player1Left.position.y += player1Left.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_W))
            {
                player1Left.position.y -= player1Left.speed * GetFrameTime();
            }

            // Set controls for player 2
            if (IsKeyDown(KEY_DOWN))
            {
                player2Right.position.y += player2Right.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_UP))
            {
                player2Right.position.y -= player2Right.speed * GetFrameTime();
            }

            // Logic for collisions of sprites
            //------------------------------------------------------------------------------------------------
            // Check for collision between player 1 and ball 1
            if (CheckCollisionCircleRec(ball1.position, ball1.radius, player1Left.GetRectangle()))      // Check collision between circle and rectangle
            {
                // If ball 1 is travelling to the left i.e. negative velocity in x
                if (ball1.velocity.x < 0)
                {
                    // Make ball 1 travel right - change its direction
                    ball1.velocity.x *= -1;
                    // If less than max velocity limits for the ball
                    if (ball1.velocity.x <= 800 || ball1.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 1 by 10%
                        ball1.velocity.x *= 1.1;
                        // Negative velocity moves up
                        // Gives an output between -1 and 1 for angle of the ball
                        ball1.velocity.y = ball1.velocity.x * GetRandomValue(-1, 1);
                        //((ball1.position.y - player1Left.position.y) / (player1Left.size.y / 2));
                    }
                }
            }

            // Check for collision between player 1 and ball 2
            if (CheckCollisionCircleRec(ball2.position, ball2.radius, player1Left.GetRectangle()))      // Check collision between circle and rectangle
            {
                // If ball 2 is travelling to the left i.e. negative velocity in x
                if (ball2.velocity.x < 0)
                {
                    // Make ball 2 travel right - change its direction
                    ball2.velocity.x *= -1;
                    // If less than max velocity limits for the ball
                    if (ball2.velocity.x <= 800 || ball2.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 2 by 10%
                        ball2.velocity.x *= 1.1;
                        // Negative velocity moves up
                        // Gives an output between -1 and 1 for angle of the ball
                        ball2.velocity.y = ball2.velocity.x * GetRandomValue(-1, 1);
                        //((ball1.position.y - player1Left.position.y) / (player1Left.size.y / 2));
                    }
                }
            }

            // Check for collision between player 2 and ball 1
            if (CheckCollisionCircleRec(ball1.position, ball1.radius, player2Right.GetRectangle()))     // Check collision between circle and rectangle
            {
                // if ball 1 is travelling to the right i.e. positive velocity in x
                if (ball1.velocity.x > 0)
                {
                    // Make ball 1 travel left - change its direction
                    ball1.velocity.x *= -1;
                    // If less than max velocity limits for the ball
                    if (ball1.velocity.x <= 800 || ball1.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 1 by 10%
                        ball1.velocity.x *= 1.1;
                        ball1.velocity.y =  (-ball1.velocity.x) * GetRandomValue(-1, 1);
                        //((ball1.position.y - player2Right.position.y) / (player2Right.size.y / 2));
                    }
                }
            }

            // Check for collision between player 2 and ball 2
            if (CheckCollisionCircleRec(ball2.position, ball2.radius, player2Right.GetRectangle()))     // Check collision between circle and rectangle
            {
                // if ball 2 is travelling to the right i.e. positive velocity in x
                if (ball2.velocity.x > 0)
                {
                    // Make ball 2 travel left - change its direction
                    ball2.velocity.x *= -1;
                    // If less than max velocity limits for the ball
                    if (ball2.velocity.x <= 800 || ball2.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 2 by 10%
                        ball2.velocity.x *= 1.1;
                        ball2.velocity.y =  (-ball2.velocity.x) * GetRandomValue(-1, 1);
                        //((ball1.position.y - player2Right.position.y) / (player2Right.size.y / 2));
                    }
                }
            }
        }
        else
        {
            // Logic for new game/round reset
            //------------------------------------------------------------------------------------------------
            if (IsKeyPressed(KEY_ENTER))
            {
                InitialiseGameObjects();
                gameWon = false;
            }
        }

        // Draw game (one frame at a time)
        BeginDrawing();
            
            ClearBackground(BLACK);

            // Draw centre court line
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, GREEN);     // Draw a line

            player1Left.Draw(BLUE);
            player2Right.Draw(RED);

            if (ball1.visible)
            {
                ball1.Draw(WHITE);
            }
            if (ball2.visible)
            {
                ball2.Draw(GOLD);
            }

            DrawText(TextFormat("%i", player1LeftScore), (screenWidth / 2) - 40, 10, 40, BLUE);     // Draw text (using default font)
            DrawText(TextFormat("%i", player2RightScore), (screenWidth / 2) + 20, 10, 40, RED);     // Draw text (using default font)

            if (player1LeftScore >= 2 || player2RightScore >= 2)
            {
                frameCounter++;             // Count the number of frames
                if (frameCounter == 60)     // Wait one second (60 frames) before making ball 2 active
                {
                    ball2.visible = true;
                }
            }

            if (player1LeftScore == 5)
            {
                DrawText("LEFT PLAYER WINS!", (screenWidth / 2) - (MeasureText("LEFT PLAYER WINS!", 50) / 2), (screenHeight / 2) - 25, 50, GOLD);
                gameWon = true;
                ball1.visible = false;
                ball2.visible = false;
            }

            if (player2RightScore == 5)
            {
                DrawText("RIGHT PLAYER WINS!", (screenWidth / 2) - (MeasureText("RIGHT PLAYER WINS!", 50) / 2), (screenHeight / 2) - 25, 50, GOLD);
                gameWon = true;
                ball1.visible = false;
                ball2.visible = false;
            }

            DrawFPS(5, 5);

            // Logic for scoring and auto ball reset
            //------------------------------------------------------------------------------------------------
            // If ball 1 passes player 2 (right)
            if (ball1.position.x > screenWidth)
            {
                player1LeftScore++;     // Player 1 scores
                ball1.Reset();          // Reset position of ball 1
            }
            // If ball 2 passes player 2 (right)
            if (ball2.position.x > screenWidth)
            {
                player1LeftScore++;     // Player 1 scores
                ball2.Reset();          // Reset position of ball 2
            }
            // If ball 1 passes player 1 (left)
            if (ball1.position.x < 0)
            {
                player2RightScore++;    // Player 2 scores
                ball1.Reset();          // Reset position of ball 1
            }
            // If ball 2 passes player 1 (left)
            if (ball2.position.x < 0)
            {
                player2RightScore++;    // Player 2 scores
                ball2.Reset();          // Reset position of ball 2
            }

        EndDrawing();
    }

    CloseWindow();
}