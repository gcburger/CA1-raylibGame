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
// Main entry point of program
//----------------------------------------------------------------------------------------------------
int main()
{
	// Initialisation
	//------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pongdemonium");

	SetTargetFPS(60);		// Set our game to run at 60 frames-per-second

	// Initialise variables of player 1
	Player player1Left;
	player1Left.position.x = 25;
	player1Left.position.y = GetScreenHeight() / 2;
	player1Left.size.x = 15;
	player1Left.size.y = 150;
	player1Left.speed = 1000;

	// Initialise variables of player 2
	Player player2Right;
	player2Right.position.x = GetScreenWidth() - 25;
	player2Right.position.y = GetScreenHeight() / 2;
	player2Right.size.x = 15;
	player2Right.size.y = 150;
	player2Right.speed = 1000;
	
	// Initialise variables of ball 1
	Ball ball1;
	ball1.position.x = GetScreenWidth() / 2;
	ball1.position.y = GetScreenHeight() / 2;
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

	// Main game loop
	while (!WindowShouldClose())		// Detect window close button or ESC key
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

		// Logic for scoring and round reset
		//------------------------------------------------------------------------------------------------
		if (IsKeyPressed(KEY_ENTER))
		{
			ball1.position.x = GetScreenWidth() / 2;
			ball1.position.y = GetScreenHeight() / 2;
			ball1.velocity.x = 400;
			ball1.velocity.y = 400;
		}

		// Draw game (one frame at a time)
		BeginDrawing();
			ClearBackground(BLACK);

			// Draw centre court line
			DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), GREEN);		// Draw a line

			player1Left.Draw(BLUE);
			player2Right.Draw(RED);
			ball1.Draw(WHITE);
			//if (ball2.active)
			//{
			//	ball2.Draw(RED);
			//}

			DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
}