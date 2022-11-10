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
const int screenHeight = 600;

//----------------------------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------------------------

int main() 
{
	// Initialisation
    //------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pongdemonium");

	SetTargetFPS(60);		// Set our game to run at 60 frames-per-second

	// Main game loop
	while (!WindowShouldClose())		// Detect window close button or ESC key
	{
		BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
}