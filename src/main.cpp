/*******************************************************************************************
 *
 *   raylib [core]
 *   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include <string>
#include <iostream>

void slideTilesLeft();
void slideTilesRight();
void slideTilesUp();
void slideTilesDown();

void drawBoard(int screenOffset, int squareSize)
{
    // grid background rectangle
    DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * 4), (squareSize * 4), DARKGRAY);

    // horizontal grid
    for (int i = 0; i < 5; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset, ((screenOffset / 2) + 150) + spacing, (squareSize * 4) + 10, 10, GRAY);
    }
    // vertical grid
    for (int i = 0; i < 5; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset + spacing, ((screenOffset / 2) + 150), 10, (squareSize * 4), GRAY);
    }
}

int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 750;
    const int screenOffset = 50;
    const int squareSize = 120;

    InitWindow(screenWidth, screenHeight, "2048");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // header text defaults
        DrawText("2048", screenOffset, screenOffset / 2, 70, LIGHTGRAY);

        DrawRectangle((screenWidth / 2) - 10, (screenOffset / 2) - 10, 155, 100, DARKGRAY);
        DrawText("HighestScore", screenWidth / 2, screenOffset / 2, 20, LIGHTGRAY);
        DrawText("CurrentScore", screenWidth / 2 + 155, screenOffset / 2, 20, LIGHTGRAY);
        // draw board
        drawBoard(screenOffset, squareSize);

        if (IsKeyPressed(KEY_LEFT))
        {
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
        }
        if (IsKeyPressed(KEY_UP))
        {
        }
        if (IsKeyPressed(KEY_DOWN))
        {
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}