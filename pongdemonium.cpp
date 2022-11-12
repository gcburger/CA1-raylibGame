/*****************************************************************************************************
*
*   Pongdemonium: a variation of the classic game
*
*   Game created using raylib v4.2 - A simple and easy-to-use library to enjoy videogames programming
*   See www.raylib.com and include/raylib.h for more details
*
*   Created by Gareth Burger (D00262405)
*
******************************************************************************************************/

#include "include/raylib.h"

//----------------------------------------------------------------------------------------------------
// Definition of constants
//----------------------------------------------------------------------------------------------------
const int screenWidth = 1000;       // Set creen width
const int screenHeight = 600;       // Set screen height

//----------------------------------------------------------------------------------------------------
// Definition of custom types
//----------------------------------------------------------------------------------------------------
// Create a structure for a ball and its variables
struct Ball
{
    Vector2 position;       // vector containing the x and y position of a ball
    Vector2 velocity;       // vector containing the x and y velocity of a ball
    float radius;           // radius of a ball
    bool visible;           // boolean to store whether a ball is active

    // Function to conveniently draw a circular ball
    void Draw(Color colour)
    {
        DrawCircle(position.x, position.y, radius, colour);     // Draw a colour filled circle
    }

    // Function to conveniently reinitialise a ball's variables to starting values
    void Reset()
    {
        position.x = screenWidth / 2;
        position.y = screenHeight / 2;
        velocity.x = 400;
        velocity.y = 400;
    }
};

// Create a structure for a player and its variables
struct Player
{
    Vector2 position;       // vector containing the x and y position of a player
    Vector2 size;           // vector containing the width and height of a player (rectangle)
    int speed;              // speed a player can move at

    // Function to conveniently draw a rectangular player
    void Draw(Color colour)
    {
        DrawRectangleRec(GetRectangle(), colour);       // Draw a colour filled rectangle
    }

    // Function to conveniently get a player, defined by it's position and size
    Rectangle GetRectangle()
    {
        return Rectangle{ position.x - (size.x / 2), position.y - (size.y / 2), size.x, size.y };
    }
};

// Create an enum of different screens to transition between
enum Screen { TITLE, CONTROLS, GAMEPLAY };

//----------------------------------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------------------------------
Player player1Left, player2Right;       // Create two player objects
Ball ball1, ball2;                      // Create two ball objects
Screen currentScreen = TITLE;           // Create screen object and initialise
int player1LeftScore, player2RightScore, frameCounter = 0, frameCounterBall2;       // Create counters for scores and frames
bool gameWon = false;                   // Create boolean to store game won state

//----------------------------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------------------------
// Set the starting values of the objects and variables in the game
// and group in a method to be able to easily reset the game on restart
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
    frameCounterBall2 = 0;
}

//----------------------------------------------------------------------------------------------------
// Main entry point of program
//----------------------------------------------------------------------------------------------------
int main()
{
    // Initialise game settings and assets
    //------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pongdemonium");      // Initialise window and OpenGL context
    InitAudioDevice();      // Initialise audio device and context

    SetTargetFPS(60);       // Set the game to run at 60 frames per second

    Sound hitBallFX = LoadSound("resources/hitBall.wav");           // Load sound from WAV file for ball and player collision
    Sound spawnBallFX = LoadSound("resources/spawnBall.wav");       // Load sound from WAV file for sound of a new ball
    Music music = LoadMusicStream("resources/8-Bit-Retro-Funk-David-Renda.mp3");    // Load sound from mp3 file for game music

    // Textures must be loaded after window initialisation (as OpenGL context is required)
    Texture2D upArrow = LoadTexture("resources/upArrow.png");       // Load texture for up arrow
    Texture2D downArrow = LoadTexture("resources/downArrow.png");   // Load texture for down arrow
    Texture2D wKey = LoadTexture("resources/w.png");                // Load texture for W key
    Texture2D sKey = LoadTexture("resources/s.png");                // Load texture for S key
    
    SetMusicVolume(music, 0.5);     // Set volume for music to 50% (1.0 is max level)
    PlayMusicStream(music);         // Play game music

    InitialiseGameObjects();        // Set the variables (position, speed etc) of game objects

    // Main game loop
    while (!WindowShouldClose())        // While game window is not closed or ESC key is not pressed
    {
        // Update game state (one frame at a time)
        //------------------------------------------------------------------------------------------------

        UpdateMusicStream(music);      // Update music buffer with new stream data

        // Allow for transitioning between 3 different game screens by switching them as required
        switch (currentScreen)
        {
            case TITLE:
            {
                frameCounter++;     // Count the number of frames that have been rendered
                // Wait 1.5 seconds (90 frames) before changing to the controls screen
                if (frameCounter > 90)
                {
                    currentScreen = CONTROLS;
                }
            }   break;
            case CONTROLS:
            {
                // Change to the gameplay screen when the user presses enter
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = GAMEPLAY;
                }
            }   break;
            case GAMEPLAY:
                // Stay on this screen for the rest of the program lifetime
                break;
            default:
                break;
        }

        // Each time a frame is rendered, if the game has not yet been won, continue playing the game
        if (!gameWon)
        {
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
            ball1.position.x += ball1.velocity.x * GetFrameTime();      // Get time in seconds from last frame drawn (delta time) i.e. amount of time between frames
            ball1.position.y += ball1.velocity.y * GetFrameTime();      // to keep change of position (velocity) in sync with frame speed

            // Set bottom bound for ball 1
            if (ball1.position.y > screenHeight - ball1.radius)
            {
                ball1.position.y = screenHeight - ball1.radius;
                // Change the direction of the ball, so that it bounces up from the bottom of the screen
                ball1.velocity.y *= -1;
            }
            // Set top bound for ball 1
            else if (ball1.position.y < 0 + ball1.radius)
            {
                ball1.position.y = 0 + ball1.radius;
                // Change the direction of the ball, so that it bounces down from the top of the screen
                ball1.velocity.y *= -1;
            }

            // If ball 2 is active, then move it around
            // Ball 1 is active from the start but ball 2 only becomes active after either player reaches a score of 3
            if (ball2.visible)
            {
                // Move ball 2 around the screen - change position by adding velocity in x and y directions
                ball2.position.x += ball2.velocity.x * GetFrameTime();      // Get time in seconds from last frame drawn (delta time) i.e. amount of time between frames
                ball2.position.y += ball2.velocity.y * GetFrameTime();      // to keep change of position (velocity) in sync with frame speed

                // Set bottom bound for ball 1
                if (ball2.position.y > screenHeight - ball2.radius)
                {
                    ball2.position.y = screenHeight - ball2.radius;
                    // Change the direction of the ball, so that it bounces up from the bottom of the screen
                    ball2.velocity.y *= -1;
                }
                // Set top bound for ball 1
                else if (ball2.position.y < 0 + ball2.radius)
                {
                    ball2.position.y = 0 + ball2.radius;
                    // Change the direction of the ball, so that it bounces down from the top of the screen
                    ball2.velocity.y *= -1;
                }
            }

            // Logic for user input controls
            //------------------------------------------------------------------------------------------------
            // Set controls for player 1
            if (IsKeyDown(KEY_S))
            {
                // Move player 1 down by increasing it's y postion by it's speed
                player1Left.position.y += player1Left.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_W))
            {
                // Move player 1 up by decreasing it's y postion by it's speed
                player1Left.position.y -= player1Left.speed * GetFrameTime();
            }

            // Set controls for player 2
            if (IsKeyDown(KEY_DOWN))
            {
                // Move player 2 down by increasing it's y postion by it's speed
                player2Right.position.y += player2Right.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_UP))
            {
                // Move player 2 up by decreasing it's y postion by it's speed
                player2Right.position.y -= player2Right.speed * GetFrameTime();
            }

            // Logic for collisions of sprites
            //------------------------------------------------------------------------------------------------
            // Check for collision between player 1 and ball 1
            if (CheckCollisionCircleRec(ball1.position, ball1.radius, player1Left.GetRectangle()))      // Check collision between circle and rectangle
            {
                // If ball 1 is travelling to the left i.e. negative x velocity
                if (ball1.velocity.x < 0)
                {
                    // Make ball 1 travel right - change its direction
                    ball1.velocity.x *= -1;
                    // If the ball's speed is less than max velocity limits for the ball (so that the ball doesn't reach unplayable speeds)
                    if (ball1.velocity.x <= 800 || ball1.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 1 by 10%
                        ball1.velocity.x *= 1.1;
                        // Give ball 1 postive or negative y velocity if it hits the top or bottom half of player 1 respectively
                        ball1.velocity.y = ball1.velocity.x * ((ball1.position.y - player1Left.position.y) / (player1Left.size.y / 2));
                    }
                }
                PlaySound(hitBallFX);      // Play WAV sound to mark the collision of ball and player
            }

            // Check for collision between player 1 and ball 2
            if (CheckCollisionCircleRec(ball2.position, ball2.radius, player1Left.GetRectangle()))      // Check collision between circle and rectangle
            {
                // If ball 2 is travelling to the left i.e. negative x velocity
                if (ball2.velocity.x < 0)
                {
                    // Make ball 2 travel right - change its direction
                    ball2.velocity.x *= -1;
                    // If the ball's speed is less than max velocity limits for the ball (so that the ball doesn't reach unplayable speeds)
                    if (ball2.velocity.x <= 800 || ball2.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 2 by 10%
                        ball2.velocity.x *= 1.1;
                        // Give ball 2 postive or negative y velocity if it hits the top or bottom half of player 1 respectively
                        ball2.velocity.y = ball2.velocity.x * ((ball2.position.y - player1Left.position.y) / (player1Left.size.y / 2));
                    }
                }
                PlaySound(hitBallFX);      // Play WAV sound to mark the collision of ball and player
            }

            // Check for collision between player 2 and ball 1
            if (CheckCollisionCircleRec(ball1.position, ball1.radius, player2Right.GetRectangle()))     // Check collision between circle and rectangle
            {
                // If ball 1 is travelling to the right i.e. positive x velocity
                if (ball1.velocity.x > 0)
                {
                    // Make ball 1 travel left - change its direction
                    ball1.velocity.x *= -1;
                    // If the ball's speed is less than max velocity limits for the ball (so that the ball doesn't reach unplayable speeds)
                    if (ball1.velocity.x <= 800 || ball1.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 1 by 10%
                        ball1.velocity.x *= 1.1;
                        // Give ball 1 postive or negative y velocity if it hits the top or bottom half of player 2 respectively
                        ball1.velocity.y =  (-ball1.velocity.x) * ((ball1.position.y - player2Right.position.y) / (player2Right.size.y / 2));
                    }
                }
                PlaySound(hitBallFX);      // Play WAV sound to mark the collision of ball and player
            }

            // Check for collision between player 2 and ball 2
            if (CheckCollisionCircleRec(ball2.position, ball2.radius, player2Right.GetRectangle()))     // Check collision between circle and rectangle
            {
                // If ball 2 is travelling to the right i.e. positive x velocity
                if (ball2.velocity.x > 0)
                {
                    // Make ball 2 travel left - change its direction
                    ball2.velocity.x *= -1;
                    // If the ball's speed is less than max velocity limits for the ball (so that the ball doesn't reach unplayable speeds)
                    if (ball2.velocity.x <= 800 || ball2.velocity.y <= 800)
                    {
                        // Increase the horizontal velocity of ball 2 by 10%
                        ball2.velocity.x *= 1.1;
                        // Give ball 2 postive or negative y velocity if it hits the top or bottom half of player 2 respectively
                        ball2.velocity.y =  (-ball2.velocity.x) * ((ball2.position.y - player2Right.position.y) / (player2Right.size.y / 2));
                    }
                }
                PlaySound(hitBallFX);      // Play WAV sound to mark the collision of ball and player
            }
        }
        else        // The game has been won (in the previous frame)
        {
            // Logic for new game/round reset
            //------------------------------------------------------------------------------------------------
            if (IsKeyPressed(KEY_ENTER))        // Game updates do not happen until the user presses enter
            {
                InitialiseGameObjects();        // Reset the game objects to their starting positions etc.
                gameWon = false;                // Reset the game won state to false i.e. new game
            }
        }

        // Draw game (one frame at a time)
        //------------------------------------------------------------------------------------------------
        BeginDrawing();     // Set up canvas (framebuffer) to start drawing

            ClearBackground(BLACK);     // Set background colour (framebuffer clear colour)

            // Depending on the current screen state, draw different game objects for each
            switch (currentScreen)
            {
                case TITLE:
                    // Draw title game text (using default font) in the middle of the screen
                    DrawText("PONGDEMONIUM", (screenWidth / 2) - (MeasureText("PONGDEMONIUM", 60) / 2), (screenHeight / 2) - 35, 60, GOLD);
                    break;
                case CONTROLS:
                {
                    // Draw control screen text (using default font) and textures relating keyboard bindings to the user in specific locations on the screen
                    DrawText("CONTROLS", (screenWidth / 2) - (MeasureText("CONTROLS", 50) / 2), (screenHeight / 8), 50, GOLD);
                    DrawText("Press ENTER", (screenWidth / 2) - (MeasureText("Press ENTER", 25) / 2), (screenHeight / 2), 25, MAGENTA);

                    DrawText("PLAYER 1", (((screenWidth / 2) - (MeasureText("Press ENTER", 25) / 2)) / 2 - 35), (screenHeight * 0.325), 30, BLUE);
                    DrawTexture(wKey, (((screenWidth / 2) - (MeasureText("Press ENTER", 25) / 2)) / 2), (screenHeight * 0.425), WHITE);
                    DrawTexture(sKey, (((screenWidth / 2) - (MeasureText("Press ENTER", 25) / 2)) / 2), (screenHeight * 0.525), WHITE);

                    DrawText("PLAYER 2", (((screenWidth / 2) - (MeasureText("Press ENTER", 25) / 2)) / 2 - 35) + screenWidth / 2, (screenHeight * 0.325), 30, RED);
                    DrawTexture(upArrow, (((screenWidth / 2) - (MeasureText("Press ENTER", 25) / 2)) / 2) + screenWidth / 2, (screenHeight * 0.425), WHITE);
                    DrawTexture(downArrow, (((screenWidth / 2) - (MeasureText("Press ENTER", 25) / 2)) / 2) + screenWidth / 2, (screenHeight * 0.525), WHITE);
                }   break;
                case GAMEPLAY:
                {
                    // Draw centre court line
                    DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, GREEN);     // Draw a line

                    player1Left.Draw(BLUE);     // Draw the rectangle for player 1 (Draw is calling DrawRectangleRec)
                    player2Right.Draw(RED);     // Draw the rectangle for player 2 (Draw is calling DrawRectangleRec)

                    // If ball 1 is active i.e. not the end of the game
                    if (ball1.visible)
                    {
                        ball1.Draw(GOLD);       // Draw the circle for ball 1 (Draw is calling DrawCircle)
                    }
                    // If ball 2 is active i.e. after either player reaches a score of 3
                    if (ball2.visible)
                    {
                        ball2.Draw(MAGENTA);    // Draw the circle for ball 2 (Draw is calling DrawCircle)
                    }

                    DrawText(TextFormat("%i", player1LeftScore), (screenWidth / 2) - 40, 10, 40, BLUE);     // Draw text to display player 1's score (using default font)
                    DrawText(TextFormat("%i", player2RightScore), (screenWidth / 2) + 20, 10, 40, RED);     // Draw text to display player 2's score (using default font)

                    // When either player 1 or player 2 reaches a score of 3
                    if (player1LeftScore >= 3 || player2RightScore >= 3)
                    {
                        frameCounterBall2++;            // Count the number of frames that have been rendered
                        if (frameCounterBall2 == 60)    // Wait one second (60 frames) before making ball 2 active
                        {
                            ball2.visible = true;       // This will allow ball 2 to be drawn on screen and move around etc.
                        }
                    }

                    // If player 1 reaches a score of 10 - they win the game
                    if (player1LeftScore == 10)
                    {
                        // Draw text informing the players of the win and how to restart
                        DrawText("PLAYER 1 WINS!", (screenWidth / 2) - (MeasureText("PLAYER 1 WINS!", 50) / 2), (screenHeight / 2) - 50, 50, GOLD);
                        DrawText("Press ENTER to play again", (screenWidth / 2) - (MeasureText("Press ENTER to play again", 25) / 2), (screenHeight / 2) + 25, 25, MAGENTA);
                        gameWon = true;             // Stop the game i.e. stop updating it, but continue to draw it
                        ball1.visible = false;      // Don't draw ball 1 until the game restarts
                        ball2.visible = false;      // Don't draw ball 2 until the game restarts
                    }
                    // If player 2 reaches a score of 10 - they win the game
                    if (player2RightScore == 10)
                    {
                        // Draw text informing the players of the win and how to restart
                        DrawText("PLAYER 2 WINS!", (screenWidth / 2) - (MeasureText("PLAYER 2 WINS!", 50) / 2), (screenHeight / 2) - 50, 50, GOLD);
                        DrawText("Press ENTER to play again", (screenWidth / 2) - (MeasureText("Press ENTER to play again", 25) / 2), (screenHeight / 2) + 25, 25, MAGENTA);
                        gameWon = true;             // Stop the game i.e. stop updating it, but continue to draw it
                        ball1.visible = false;      // Don't draw ball 1 until the game restarts
                        ball2.visible = false;      // Don't draw ball 2 until the game restarts
                    }

                    // Logic for scoring and auto ball reset
                    //------------------------------------------------------------------------------------------------
                    // If ball 1 passes player 2 (exits the screen on the right)
                    if (ball1.position.x > screenWidth)
                    {
                        player1LeftScore++;         // Player 1 scores
                        ball1.Reset();              // Reset position of ball 1
                        PlaySound(spawnBallFX);     // Play WAV sound to mark ball 1 coming back into play
                    }
                    // If ball 2 passes player 2 (exits the screen on the right)
                    if (ball2.position.x > screenWidth)
                    {
                        player1LeftScore++;         // Player 1 scores
                        ball2.Reset();              // Reset position of ball 2
                        PlaySound(spawnBallFX);     // Play WAV sound to mark ball 2 coming back into play
                    }
                    // If ball 1 passes player 1 (exits the screen on the left)
                    if (ball1.position.x < 0)
                    {
                        player2RightScore++;        // Player 2 scores
                        ball1.Reset();              // Reset position of ball 1
                        PlaySound(spawnBallFX);     // Play WAV sound to mark ball 1 coming back into play
                    }
                    // If ball 2 passes player 1 (exits the screen on the left)
                    if (ball2.position.x < 0)
                    {
                        player2RightScore++;        // Player 2 scores
                        ball2.Reset();              // Reset position of ball 2
                        PlaySound(spawnBallFX);     // Play WAV sound to mark ball 2 coming back into play
                    }
                }   break;
                default:
                    break;
            }

        EndDrawing();       // End canvas drawing and swap buffers (double buffering)
    }

    // Deinitialise game
    //------------------------------------------------------------------------------------------------
    UnloadSound(hitBallFX);         // Unload hitBallFX sound data
    UnloadSound(spawnBallFX);       // Unload spawnBallFX sound data
    UnloadMusicStream(music);       // Unload music stream from RAM

    UnloadTexture(upArrow);         // Unload upArrow texture from GPU memory (VRAM)
    UnloadTexture(downArrow);       // Unload downArrow texture from GPU memory (VRAM)
    UnloadTexture(wKey);            // Unload wKey texture from GPU memory (VRAM)
    UnloadTexture(sKey);            // Unload sKey texture from GPU memory (VRAM)

    CloseAudioDevice();     // Close the audio device and context

    CloseWindow();          // Close window and unload OpenGL context
}