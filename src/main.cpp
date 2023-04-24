/*******************************************************************************************
 *
 *   raylib [core] example - Basic windows
 *   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"

int main(void)
{
    const int screenWidth = 700;
    const int screenHeight = 600;
    const int screenOffset = 50;

    InitWindow(screenWidth, screenHeight, "2048");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // header text defaults
        DrawText("2048", screenOffset, screenOffset / 2, 70, LIGHTGRAY);
        DrawText("HighestScore", screenWidth / 2, screenOffset / 2, 20, LIGHTGRAY);
        DrawText("CurrentScore", screenWidth / 2 + 150, screenOffset / 2, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}