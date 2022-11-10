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
	Vector2 speed;
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
		return Rectangle{ position.x - (size.x / 2), position.y - (size.y / 2), 15, 100 };
	}
};

//----------------------------------------------------------------------------------------------------
// Program main entry point
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
	player1Left.size.y = 100;
	player1Left.speed = 400;

	// Initialise variables of player 2
	Player player2Right;
	player2Right.position.x = GetScreenWidth() - 25;
	player2Right.position.y = GetScreenHeight() / 2;
	player2Right.size.x = 15;
	player2Right.size.y = 100;
	player2Right.speed = 400;
	
	// Initialise variables of ball 1
	Ball ball1;
	ball1.position.x = GetScreenWidth() / 2;
	ball1.position.y = GetScreenHeight() / 2;
	ball1.radius = 10;
	ball1.speed.x = 250;
	ball1.speed.y = 250;
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
		BeginDrawing();
			ClearBackground(BLACK);

			player1Left.Draw(WHITE);
			player2Right.Draw(RED);
			ball1.Draw(BLUE);
			//if (ball2.active)
			//{
			//	ball2.Draw(RED);
			//}

			DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
}