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
    bool active;

    void Draw(Color colour)
	{
		DrawCircle(position.x, position.y, radius, colour);		// Draw a color-filled circle
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
		DrawRectangleRec(GetRectangle(), colour);		// Draw a color-filled rectangle
	}

	Rectangle GetRectangle()
	{
		return Rectangle{ position.x - (size.x / 2), position.y - (size.y / 2), size.x, size.y };
	}
};

//----------------------------------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------------------------------
Player player1Left;
Player player2Right;
Ball ball1;
int player1LeftScore = 0;
int player2RightScore = 0;
bool gamePaused = false;
bool gameWon = false;

static void InitGame(void);

//----------------------------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------------------------
void InitialiseGame(void)
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
	ball1.active = true;

	// Initialise variables of ball 2
	/*Ball ball2;
	ball2.position.x = GetScreenWidth() / 2;
	ball2.position.y = GetScreenHeight() / 2;
	ball2.radius = 10;
	ball2.speed.x = 250;
	ball2.speed.y = 250;
	ball2.active = false;*/
}

//----------------------------------------------------------------------------------------------------
// Main entry point of program
//----------------------------------------------------------------------------------------------------
int main(void)
{
	// Initialisation
	//------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pongdemonium");

	SetTargetFPS(60);		// Set our game to run at 60 frames-per-second

	InitialiseGame();

	// Main game loop
	while (!WindowShouldClose())		// Detect window close button or ESC key
	{
		if (!gameWon)
		{
			// Update game state (one frame at a time)
			
			// Logic for position of game objects (sprites)
			//------------------------------------------------------------------------------------------------
			// Set bottom bound for player 1
			if (player1Left.position.y > GetScreenHeight() - (player1Left.size.y / 2))
			{
				player1Left.position.y = GetScreenHeight() - (player1Left.size.y / 2);
			}
			// Set top bound for player 1
			else if (player1Left.position.y < 0 + (player1Left.size.y / 2))
			{
				player1Left.position.y = 0 + (player1Left.size.y / 2);
			}

			// Set bottom bound for player 2
			if (player2Right.position.y > GetScreenHeight() - (player2Right.size.y / 2))
			{
				player2Right.position.y = GetScreenHeight() - (player2Right.size.y / 2);
			}
			// Set top bound for player 2
			else if (player2Right.position.y < 0 + (player2Right.size.y / 2))
			{
				player2Right.position.y = 0 + (player2Right.size.y / 2);
			}

			// Move ball 1 around the screen - change position by adding velocity in x and y directions
			ball1.position.x += ball1.velocity.x * GetFrameTime();		// Get time in seconds for last frame drawn (delta time) i.e. amount of time between frames
			ball1.position.y += ball1.velocity.y * GetFrameTime();		// to keep change of position (velocity) in sync with frame speed

			// Set bottom bound for ball 1
			if (ball1.position.y > GetScreenHeight() - ball1.radius)
			{
				ball1.position.y = GetScreenHeight() - ball1.radius;
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

			// insert bounds for ball2 here

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
			if (CheckCollisionCircleRec(ball1.position, ball1.radius, player1Left.GetRectangle()))		// Check collision between circle and rectangle
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
			// Check for collision between player 2 and ball 1
			if (CheckCollisionCircleRec(ball1.position, ball1.radius, player2Right.GetRectangle()))		// Check collision between circle and rectangle
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
		}
		else
		{
			// Logic for scoring and round reset
			//------------------------------------------------------------------------------------------------
			if (IsKeyPressed(KEY_ENTER))
			{
				InitialiseGame();
				gameWon = false;
			}
		}

		// Draw game (one frame at a time)
		BeginDrawing();
			
			ClearBackground(BLACK);

			// Draw centre court line
			DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, GREEN);		// Draw a line

			player1Left.Draw(BLUE);
			player2Right.Draw(RED);
			ball1.Draw(WHITE);
			//if (ball2.active)
			//{
			//	ball2.Draw(GOLD);
			//}

			DrawText(TextFormat("%i", player1LeftScore), (screenWidth / 2) - 40, 10, 40, BLUE);		// Draw text (using default font)
			DrawText(TextFormat("%i", player2RightScore), (screenWidth / 2) + 20, 10, 40, RED);		// Draw text (using default font)

			DrawFPS(5, 5);

			// Logic for scoring and round reset
			//------------------------------------------------------------------------------------------------
			if (ball1.position.x > screenWidth)
			{
				player1LeftScore++;
				ball1.Reset();
				if (player1LeftScore == 3)
				{
					DrawText("LEFT PLAYER WINS!", (screenWidth / 2) - (MeasureText("LEFT PLAYER WINS!", 60) / 2), screenHeight / 2, 60, GOLD);
					gameWon = true;
					player1LeftScore = 0;
					player2RightScore = 0;
				}
			}
			if (ball1.position.x < 0)
			{
				player2RightScore++;
				ball1.Reset();
				if (player2RightScore == 3)
				{
					DrawText("RIGHT PLAYER WINS!", (screenWidth / 2) - (MeasureText("RIGHT PLAYER WINS!", 60) / 2), screenHeight / 2, 60, GOLD);
					gameWon = true;
					player1LeftScore = 0;
					player2RightScore = 0;
				}
			}

		EndDrawing();
		}

	CloseWindow();
}