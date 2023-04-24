/*******************************************************************************************
 *
 *   raylib [core]
 *   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include <string>
#include <iostream>

struct tile
{
    Vector2 position;
    Color color;

} squareTile;

// --------------------
// GLOBAL VARIABLES
const int screenWidth = 600;
const int screenHeight = 750;
const int screenOffset = 50;
const int squareSize = 120;
const int lineWidth = 10;
const int tileSize = squareSize - 10;

static bool gameOver = false;

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
        DrawRectangle(screenOffset, ((screenOffset / 2) + 150) + spacing, (squareSize * 4) + lineWidth, lineWidth, GRAY);
    }
    // vertical grid
    for (int i = 0; i < 5; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset + spacing, ((screenOffset / 2) + 150), lineWidth, (squareSize * 4), GRAY);
    }
}

void initGame()
{
    // initializing defalut values before running game
    InitWindow(screenWidth, screenHeight, "2048");

    SetTargetFPS(60);

    // set default values for tile
    squareTile.position.x = screenOffset + 10;
    squareTile.position.y = ((screenOffset / 2) + 150) + lineWidth;
}

int main(void)
{
    initGame();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_LEFT))
        {
            squareTile.position.x -= squareSize;
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            squareTile.position.x += squareSize;
        }
        if (IsKeyPressed(KEY_UP))
        {
            squareTile.position.y -= squareSize;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            squareTile.position.y += squareSize;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // header text defaults
        DrawText("2048", screenOffset, screenOffset / 2, 80, LIGHTGRAY);

        DrawRectangle(screenOffset + lineWidth + 3 * squareSize, (screenOffset / 2) - lineWidth, tileSize, tileSize, BROWN);
        DrawText("Score", screenOffset + (2 * lineWidth) + (3 * squareSize), screenOffset / 2, 20, LIGHTGRAY);

        // draw board
        drawBoard(screenOffset, squareSize);

        // // tile
        DrawRectangle(squareTile.position.x, squareTile.position.y, tileSize, tileSize, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}