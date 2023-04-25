/*******************************************************************************************
 *
 *   raylib [core]
 *   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include <vector>

#include <string>
#include <iostream>

struct grid
{
    std::vector<std::vector<Vector2>> gridPosition = {
        {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 4}},
        {{1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}},
        {{2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}},
        {{3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}},
        {{4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}},
        {{5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}},
    };

    std::vector<std::vector<bool>> gridFilled = {
        {true, true, true, true, true, true},
        {true, false, false, false, false, true},
        {true, false, false, false, false, true},
        {true, false, false, false, false, true},
        {true, false, false, false, false, true},
        {true, true, true, true, true, true}};
} Grid;

struct tile
{
    Vector2 position;
    Vector2 positionGrid;
    Color color;
    grid tileGrid;

} squareTile;

// create a vector that stores all the squareTiles
std::vector<tile> totalTiles;

// --------------------
// GLOBAL VARIABLES
const int screenWidth = 600;
const int screenHeight = 750;
const int screenOffset = 50;
const int squareSize = 120;
const int lineWidth = 10;
const int tileSize = squareSize - lineWidth;

static bool gameOver = false;

void slideTilesLeft();
void slideTilesRight();
void slideTilesUp();
void slideTilesDown();
bool isGridOccupied(std::vector<int>);

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

// Generate tile

void initGame()
{
    // initializing defalut values before running game
    InitWindow(screenWidth, screenHeight, "2048");

    SetTargetFPS(60);

    // set default values for tile
    squareTile.position.x = screenOffset + lineWidth;
    squareTile.position.y = ((screenOffset / 2) + 150) + lineWidth;
    squareTile.positionGrid.x = 1;
    squareTile.positionGrid.y = 1;
}

int main(void)
{
    initGame();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_LEFT))
        {
            for (int i = 1; i < 5; i++)
            {
                if (Grid.gridFilled[squareTile.positionGrid.x - 1][squareTile.positionGrid.y] == false)
                {
                    squareTile.position.x -= squareSize;
                    squareTile.positionGrid.x -= 1;
                    std::cout << squareTile.positionGrid.x << " , " << squareTile.positionGrid.y << std::endl;
                }
            }
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            for (int i = 1; i < 5; i++)
            {
                if (Grid.gridFilled[squareTile.positionGrid.x + 1][squareTile.positionGrid.y] == false)
                {
                    squareTile.position.x += squareSize;
                    squareTile.positionGrid.x += 1;
                    std::cout << squareTile.positionGrid.x << " , " << squareTile.positionGrid.y << std::endl;
                }
            }
        }
        if (IsKeyPressed(KEY_UP))
        {
            for (int i = 1; i < 5; i++)
            {
                if (Grid.gridFilled[squareTile.positionGrid.x][squareTile.positionGrid.y - 1] == false)
                {
                    squareTile.position.y -= squareSize;
                    squareTile.positionGrid.y -= 1;
                    std::cout << squareTile.positionGrid.x << " , " << squareTile.positionGrid.y << std::endl;
                }
            }
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            for (int i = 1; i < 5; i++)
            {
                if (Grid.gridFilled[squareTile.positionGrid.x][squareTile.positionGrid.y + 1] == false)
                {
                    squareTile.position.y += squareSize;
                    squareTile.positionGrid.y += 1;
                    std::cout << squareTile.positionGrid.x << " , " << squareTile.positionGrid.y << std::endl;
                }
            }
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