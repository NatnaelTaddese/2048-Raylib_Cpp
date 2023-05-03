/*******************************************************************************************
 *
 *   raylib [core]
 *
 ********************************************************************************************/

#include "raylib.h"
#include <vector>
#include <array>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

// GLOBAL VARIABLES
const int screenWidth = 600;
const int screenHeight = 750;
const int screenOffset = 50;
const int squareSize = 120;
const int lineWidth = 10;
const int tileSize = squareSize - lineWidth;

struct
{
    Color backgroundColor = DARKGRAY;
    Color gridColor = GRAY;

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
    Vector2 absolutePosition;
    Vector2 relativePosition;
    Color color;
    int numValue = 0;
    bool isOccupied = false;

} defaultTile;

// vector that stores all the squareTiles
std::array<std::array<tile, 4>, 4> totalTile;

// Generate tile
void generateTile()
{
    int x, y;

    do
    {
        x = rand() % 5;
        y = rand() % 5;
    } while (Grid.gridFilled[x][y]);

    tile tiles;
    tiles.relativePosition.x = x;
    tiles.relativePosition.y = y;
    tiles.absolutePosition.x = (screenOffset + lineWidth) + ((x - 1) * squareSize);
    tiles.absolutePosition.y = (((screenOffset / 2) + 150) + lineWidth) + ((y - 1) * squareSize);
    tiles.numValue = 2;
    tiles.isOccupied = true;

    // occupy the gird
    Grid.gridFilled[x][y] = true;

    // append to the total tiles vector
    totalTile[x - 1][y - 1] = tiles;
}

// draw all tiles
inline void DrawTiles(std::array<std::array<tile, 4>, 4> &totalTile)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (totalTile[i][j].numValue != 0)
            {

                DrawRectangle(totalTile[i][j].absolutePosition.x, totalTile[i][j].absolutePosition.y, tileSize, tileSize, LIGHTGRAY);
                DrawText(std::to_string(totalTile[i][j].numValue).c_str(), (totalTile[i][j].absolutePosition.x + (tileSize / 2) - 10), (totalTile[i][j].absolutePosition.y + (tileSize / 2) - 30), 60, RED);
            }
        }
    }
}

// void slideTilesLeft(std::array<std::array<tile, 4>, 4> &totalTile)
// {

//     for (int i = 0; i < 4; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//            for (int i = 1; i < 5; i++)
//         {
//             if (Grid.gridFilled[totalTile[i][j].relativePosition.x - 1][totalTile[i][j].relativePosition.y] == false)
//             {
//                 // std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

//                 Grid.gridFilled[totalTile[i][j].relativePosition.x][totalTile[i][j].relativePosition.y] = false;

//                 tile swap;
//                 totalTile[i][j] = swap;
//                 totalTile[i][j] = totalTile[i][j - 1];

//                 // update grid fillment

//                 // std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

//                 // std::cout << t.relativePosition.x << " , " << t.relativePosition.y << std::endl;
//             }
//         }

//         }
//     }
// }

void slideTilesLeft(std::array<std::array<tile, 4>, 4> &totalTile)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // if (totalTile[i][j].numValue == 0)
            // {
            //     continue;
            // }
            if (Grid.gridFilled[i + 1][j] == false)
            {
                std::cout << "Left Possible for " << i << j << std::endl;
                // Grid.gridFilled[totalTile[i][j].relativePosition.x][totalTile[i][j].relativePosition.y] = false;

                // totalTile[i][j].absolutePosition.x -= squareSize;
                // totalTile[i][j].relativePosition.x -= 1;

                // totalTile[i][j - 1] = totalTile[i][j];
                // totalTile[i][j] = defaultTile;
                // Grid.gridFilled[totalTile[i][j - 1].relativePosition.x][totalTile[i][j - 1].relativePosition.y] = true;
            }
        }
    }
}

void slideTilesLeft(std::vector<tile> &tiles)
{

    for (tile &t : tiles)
    {
        for (int i = 1; i < 5; i++)
        {
            if (Grid.gridFilled[t.relativePosition.x - 1][t.relativePosition.y] == false)
            {
                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = false;
                t.absolutePosition.x -= squareSize;
                t.relativePosition.x -= 1;

                // update grid fillment

                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                std::cout << t.relativePosition.x << " , " << t.relativePosition.y << std::endl;
            }
        }
        Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = true;
        // std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;
    }
}
void slideTilesRight(std::vector<tile> &tiles)
{

    for (tile &t : tiles)
    {
        for (int i = 1; i < 5; i++)
        {
            if (Grid.gridFilled[t.relativePosition.x + 1][t.relativePosition.y] == false)
            {
                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = false;
                t.absolutePosition.x += squareSize;
                t.relativePosition.x += 1;

                // update grid fillment

                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                std::cout << t.relativePosition.x << " , " << t.relativePosition.y << std::endl;
            }
        }
        Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = true;
        std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;
    }
}
void slideTilesUp(std::vector<tile> &tiles)
{
    for (tile &t : tiles)
    {
        for (int i = 1; i < 5; i++)
        {
            if (Grid.gridFilled[t.relativePosition.x][t.relativePosition.y - 1] == false)
            {
                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = false;
                t.absolutePosition.y -= squareSize;
                t.relativePosition.y -= 1;

                // update grid fillment

                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                std::cout << t.relativePosition.x << " , " << t.relativePosition.y << std::endl;
            }
        }
        Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = true;
        std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;
    }
}
void slideTilesDown(std::vector<tile> &tiles)
{
    for (tile &t : tiles)
    {
        for (int i = 1; i < 5; i++)
        {
            if (Grid.gridFilled[t.relativePosition.x][t.relativePosition.y + 1] == false)
            {
                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = false;
                t.absolutePosition.y += squareSize;
                t.relativePosition.y += 1;

                // update grid fillment

                std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;

                std::cout << t.relativePosition.x << " , " << t.relativePosition.y << std::endl;
            }
        }
        Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] = true;
        std::cout << Grid.gridFilled[t.relativePosition.x][t.relativePosition.y] << std::endl;
    }
}

// sum tiles according to the movement applied
void sumTilesleft(std::vector<tile> &tiles);
void sumTilesRight(std::vector<tile> &tiles);
void sumTilesUp(std::vector<tile> &tiles);
void sumTilesDown(std::vector<tile> &tiles);

// draw the playing square board
void drawBoard(int screenOffset, int squareSize)
{
    // grid background rectangle
    DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * 4), (squareSize * 4), Grid.backgroundColor);

    // horizontal grid
    for (int i = 0; i < 5; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset, ((screenOffset / 2) + 150) + spacing, (squareSize * 4) + lineWidth, lineWidth, Grid.gridColor);
    }
    // vertical grid
    for (int i = 0; i < 5; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset + spacing, ((screenOffset / 2) + 150), lineWidth, (squareSize * 4), Grid.gridColor);
    }
}

void initGame()
{
    // initializing defalut values before running game
    InitWindow(screenWidth, screenHeight, "2048");

    SetTargetFPS(60);

    // fill the array with default tile
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            totalTile[i][j] = defaultTile;
        }
    }
    generateTile();
    generateTile();
    generateTile();
    generateTile();
    generateTile();
}

void drawHeader()
{
    // header text defaults
    DrawText("2048", screenOffset, screenOffset / 2, 80, LIGHTGRAY);

    // score board
    DrawRectangle(screenOffset + lineWidth + 3 * squareSize, (screenOffset / 2) - lineWidth, tileSize, tileSize, BROWN);
    DrawText("Score", screenOffset + (2 * lineWidth) + (3 * squareSize), screenOffset / 2, 20, LIGHTGRAY);
}

int main(void)
{
    initGame();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_LEFT))
        {
            // for (int i = 0; i < 4; i++)
            // {
            slideTilesLeft(totalTile);
            // }
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            for (int i = 0; i < 4; i++)
            {
                // slideTilesRight(totalTile);
            }
        }
        if (IsKeyPressed(KEY_UP))
        {
            for (int i = 0; i < 4; i++)
            {
                // slideTilesUp(totalTile);
            }
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            for (int i = 0; i < 4; i++)
            {
                // slideTilesDown(totalTile);
            }
        }
        // for debugging
        if (IsKeyPressed(KEY_G))
        {
            generateTile();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // draw header
        drawHeader();

        // draw board
        drawBoard(screenOffset, squareSize);

        // // tile
        DrawTiles(totalTile);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}