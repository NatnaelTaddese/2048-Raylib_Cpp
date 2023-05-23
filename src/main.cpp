/*******************************************************************************************
 *
 *   raylib [core]
 *
 ********************************************************************************************/

#include "raylib.h"
#include <array>
#include <string>
#include <cmath>
// #include <iostream>
#include <fstream>

Color TILECOLOR2 = (Color){238, 228, 218, 255};
Color TILECOLOR4 = (Color){238, 225, 201, 255};
Color TILECOLOR8 = (Color){243, 178, 122, 255};
Color TILECOLOR16 = (Color){246, 150, 100, 255};
Color TILECOLOR32 = (Color){247, 124, 95, 255};
Color TILECOLOR64 = (Color){247, 95, 59, 255};
Color TILECOLOR128 = (Color){237, 208, 115, 255};
Color TILECOLOR256 = (Color){247, 180, 98, 255};

Color NUMCOLOR = (Color){249, 246, 242, 255};
Color NUMCOLORDARK = (Color){119, 110, 101, 255};

Color BGCOLOR = (Color){205, 193, 180, 255};
Color BGCOLOR_SEMI_TRANS = (Color){205, 193, 180, 70};
Color GRIDCOLOR = (Color){187, 173, 160, 255};

// GLOBAL VARIABLES
const int screenWidth = 600;
const int screenHeight = 750;
const int screenOffset = 50;
const int squareSize = 120;
const int lineWidth = 10;
const int tileSize = squareSize - lineWidth;
int fixFontPosition;
int fixFontSize;
int currentScore;
int highScore;
double lastUpdateTime = 0;
bool moveValid = false;
bool splashScreen = true;
Color tileColor;
Color numColor;

float lerp(float a, float b, float t)
{
    return a + (b - a) * t; // This returns a + t percent (t = 0.f is 0% and t = 1.f is 100%) of b
}

Vector2 Lerp(const Vector2 &v1, const Vector2 &v2, float t)
{
    float x = v1.x + (v2.x - v1.x) * t;
    float y = v1.y + (v2.y - v1.y) * t;
    Vector2 lerped;
    lerped.x = x;
    lerped.y = y;
    return lerped;
}

struct
{
    Color backgroundColor = DARKGRAY;
    Color gridColor = GRAY;
} Grid;

struct tile
{
    Vector2 absolutePosition;
    Color tileColor;
    int numValue = 0;
    bool isOccupied = false;
    bool isNew;
    bool isSliding;
    float tileAnimationProgress;

} defaultTile;

// vector that stores all the squareTiles
std::array<std::array<tile, 4>, 4> totalTile;
std::array<std::array<tile, 4>, 4> last_move;

// Generate tile
void generateTile()
{
    int x, y, r_num;

    do
    {
        x = rand() % 4;
        y = rand() % 4;
        r_num = (rand() % 2) + 1;
    } while (totalTile[y][x].isOccupied);

    tile tiles;
    tiles.absolutePosition.x = (screenOffset + lineWidth) + ((x)*squareSize);
    tiles.absolutePosition.y = (((screenOffset / 2) + 150) + lineWidth) + ((y)*squareSize);
    tiles.numValue = 2 * r_num;
    tiles.isOccupied = true;
    tiles.isNew = true;
    tiles.tileAnimationProgress = 0;

    // append to the total tiles vector
    totalTile[y][x] = tiles;
    // std::cout << "Generated: " << y << x << "  " << totalTile[y][x].isNew << std::endl;
    moveValid = false;
    lastUpdateTime = 0;
}

bool is_game_over(std::array<std::array<tile, 4>, 4> &totalTile)
{
    // Check if there are any empty tiles
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (totalTile[i][j].numValue == 0)
            {
                return false;
            }
        }
    }

    // Check if there are any adjacent tiles with the same value
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((i < 3 && totalTile[i][j].numValue == totalTile[i + 1][j].numValue) ||
                (j < 3 && totalTile[i][j].numValue == totalTile[i][j + 1].numValue))
            {
                return false;
            }
        }
    }

    // No moves available, game is over
    return true;
}

void updateHighScore()
{
    std::ofstream fout("../bin/h_score.bin", std::ios::binary);

    fout.write(reinterpret_cast<char *>(&currentScore), sizeof(currentScore));

    // Close binary file
    fout.close();
}

void readScore()
{
    std::ifstream fin("../bin/h_score.bin", std::ios::binary);

    if (!fin.is_open())
    {
        std::ofstream fout("../bin/h_score.bin", std::ios::binary);
        fout.write(reinterpret_cast<char *>(&currentScore), sizeof(currentScore));

        // Close binary file
        fout.close();
    }

    // Read number from binary file
    int h_score;
    fin.read(reinterpret_cast<char *>(&h_score), sizeof(h_score));
    highScore = h_score;

    // std::cout << h_score << std::endl;

    // Close binary file
    fin.close();

    if (highScore < currentScore)
    {
        updateHighScore();
    }

    // std::cout << "file read successfuly" << std::endl;
}

void checkScore()
{

    if (highScore < currentScore)
    {
        readScore();
    }
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

                fixFontPosition = totalTile[i][j].numValue < 10 ? 0 : totalTile[i][j].numValue < 20 ? 10
                                                                  : totalTile[i][j].numValue < 100  ? 20
                                                                  : totalTile[i][j].numValue < 1000 ? 30
                                                                                                    : 40;

                fixFontSize = totalTile[i][j].numValue < 100 ? 0 : totalTile[i][j].numValue < 300 ? 5
                                                               : totalTile[i][j].numValue < 600   ? 6
                                                               : totalTile[i][j].numValue < 1200  ? 15
                                                                                                  : 20;
                tileColor = totalTile[i][j].numValue == 2 ? TILECOLOR2 : totalTile[i][j].numValue == 4 ? TILECOLOR4
                                                                     : totalTile[i][j].numValue == 8   ? TILECOLOR8
                                                                     : totalTile[i][j].numValue == 16  ? TILECOLOR16
                                                                     : totalTile[i][j].numValue == 32  ? TILECOLOR32
                                                                     : totalTile[i][j].numValue == 64  ? TILECOLOR64
                                                                     : totalTile[i][j].numValue == 128 ? TILECOLOR128
                                                                     : totalTile[i][j].numValue == 256 ? TILECOLOR256
                                                                                                       : TILECOLOR128;

                numColor = totalTile[i][j].numValue < 16 ? NUMCOLORDARK : NUMCOLOR;

                if (totalTile[i][j].isNew)
                {
                    totalTile[i][j].tileAnimationProgress += 0.05f;

                    // Calculate current tile position using Lerp function
                    Vector2 currentTilePos = Lerp(totalTile[i][j].absolutePosition,
                                                  {(screenOffset + lineWidth) + ((j)*squareSize),
                                                   (((screenOffset / 2) + 150) + lineWidth) + ((i)*squareSize)},
                                                  totalTile[i][j].tileAnimationProgress);

                    // Draw tile at current position
                    DrawRectangleV(currentTilePos, (Vector2){tileSize * totalTile[i][j].tileAnimationProgress, tileSize}, tileColor);

                    // If tile animation is complete, set isNew flag to false
                    if (totalTile[i][j].tileAnimationProgress >= 1.0f)
                    {
                        totalTile[i][j].isNew = false;
                    }
                }

                else if (totalTile[i][j].isSliding)
                {
                    totalTile[i][j].tileAnimationProgress += 0.05f;

                    // Calculate current tile position using Lerp function
                    Vector2 currentTilePos = Lerp(totalTile[i][j].absolutePosition,
                                                  {(screenOffset + lineWidth) + ((j)*squareSize),
                                                   (((screenOffset / 2) + 150) + lineWidth) + ((i)*squareSize)},
                                                  totalTile[i][j].tileAnimationProgress);

                    // Draw tile at current position
                    DrawRectangleV(currentTilePos, (Vector2){tileSize * totalTile[i][j].tileAnimationProgress, tileSize}, tileColor);

                    // If tile animation is complete, set isNew flag to false
                    if (totalTile[i][j].tileAnimationProgress >= 1.0f)
                    {
                        totalTile[i][j].isSliding = false;
                    }
                }

                else
                {
                    DrawRectangle(totalTile[i][j].absolutePosition.x, totalTile[i][j].absolutePosition.y, tileSize, tileSize, tileColor);
                    DrawText(std::to_string(totalTile[i][j].numValue).c_str(), (totalTile[i][j].absolutePosition.x + (tileSize / 2) - 10) - fixFontPosition, (totalTile[i][j].absolutePosition.y + (tileSize / 2) - 30), 60 - fixFontSize, numColor);
                    totalTile[i][j].isNew = false;
                }
            }
        }
    }
}

void slideTilesLeft(std::array<std::array<tile, 4>, 4> &totalTile)
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {

            if (totalTile[i][j].isOccupied && totalTile[i][j - 1].isOccupied == false)
            {

                totalTile[i][j].absolutePosition.x -= squareSize;

                totalTile[i][j - 1] = totalTile[i][j];
                totalTile[i][j] = defaultTile;

                moveValid = true;
                totalTile[i][j - 1].isSliding = true;
            }
        }
    }
}
void slideTilesRight(std::array<std::array<tile, 4>, 4> &totalTile)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 2; j >= 0; j--)
        {

            if (totalTile[i][j].isOccupied && totalTile[i][j + 1].isOccupied == false)
            {

                totalTile[i][j].absolutePosition.x += squareSize;

                totalTile[i][j + 1] = totalTile[i][j];
                totalTile[i][j] = defaultTile;
                moveValid = true;
            }
        }
    }
}
void slideTilesUp(std::array<std::array<tile, 4>, 4> &totalTile)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {

            if (totalTile[j][i].isOccupied && totalTile[j - 1][i].isOccupied == false)
            {

                totalTile[j][i].absolutePosition.y -= squareSize;

                totalTile[j - 1][i] = totalTile[j][i];
                totalTile[j][i] = defaultTile;
                moveValid = true;
            }
        }
    }
}
void slideTilesDown(std::array<std::array<tile, 4>, 4> &totalTile)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 2; j >= 0; j--)
        {

            if (totalTile[j][i].isOccupied && totalTile[j + 1][i].isOccupied == false)
            {

                totalTile[j][i].absolutePosition.y += squareSize;

                totalTile[j + 1][i] = totalTile[j][i];
                totalTile[j][i] = defaultTile;
                moveValid = true;
            }
        }
    }
}

// sum tiles according to the movement applied
void sumTilesleft(std::array<std::array<tile, 4>, 4> &totalTile)
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {

            if ((totalTile[i][j].isOccupied && totalTile[i][j - 1].isOccupied) && (totalTile[i][j].numValue == totalTile[i][j - 1].numValue))
            {
                currentScore += totalTile[i][j - 1].numValue;

                totalTile[i][j - 1].numValue *= 2;

                totalTile[i][j] = defaultTile;
            }
        }
    }
    slideTilesLeft(totalTile);
}
void sumTilesRight(std::array<std::array<tile, 4>, 4> &totalTile)
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 2; j >= 0; j--)
        {

            if ((totalTile[i][j].isOccupied && totalTile[i][j + 1].isOccupied) && (totalTile[i][j].numValue == totalTile[i][j + 1].numValue))
            {
                currentScore += totalTile[i][j + 1].numValue;

                totalTile[i][j + 1].numValue *= 2;

                totalTile[i][j] = defaultTile;
            }
        }
    }
    slideTilesRight(totalTile);
}
void sumTilesUp(std::array<std::array<tile, 4>, 4> &totalTile)
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {

            if ((totalTile[j][i].isOccupied && totalTile[j - 1][i].isOccupied) && (totalTile[j][i].numValue == totalTile[j - 1][i].numValue))
            {
                currentScore += totalTile[j - 1][i].numValue;

                totalTile[j - 1][i].numValue *= 2;
                totalTile[j][i] = defaultTile;
            }
        }
    }
    slideTilesUp(totalTile);
}
void sumTilesDown(std::array<std::array<tile, 4>, 4> &totalTile)
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 2; j >= 0; j--)
        {

            if ((totalTile[j][i].isOccupied && totalTile[j + 1][i].isOccupied) && (totalTile[j][i].numValue == totalTile[j + 1][i].numValue))
            {
                currentScore += totalTile[j + 1][i].numValue;

                totalTile[j + 1][i].numValue *= 2;
                totalTile[j][i] = defaultTile;
            }
        }
    }
    slideTilesDown(totalTile);
}

// draw the playing square board
void drawBoard(int screenOffset, int squareSize)
{
    // grid background rectangle
    DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * 4), (squareSize * 4), BGCOLOR);

    // horizontal grid
    for (int i = 0; i < 5; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset, ((screenOffset / 2) + 150) + spacing, (squareSize * 4) + lineWidth, lineWidth, GRIDCOLOR);
    }
    // vertical grid
    for (int i = 0; i < 5; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset + spacing, ((screenOffset / 2) + 150), lineWidth, (squareSize * 4), GRIDCOLOR);
    }
}

void initGame()
{
    // initializing defalut values before running game

    currentScore = 0;
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
    readScore();
    readScore();
}

void drawHeader()
{
    // header text defaults
    DrawText("2048", screenOffset, screenOffset / 2 - lineWidth, 62, LIGHTGRAY);

    // score board
    DrawRectangle(screenOffset + lineWidth + 3 * squareSize, (screenOffset / 2) - lineWidth, tileSize, tileSize / 2, BROWN);
    DrawText("Score", screenOffset + (2 * lineWidth) + (3 * squareSize), screenOffset / 2, 20, LIGHTGRAY);
    DrawText(std::to_string(currentScore).c_str(), screenOffset + (2 * lineWidth) + (3 * squareSize), (screenOffset / 2) + 20, 20, LIGHTGRAY);

    DrawRectangle(screenOffset + lineWidth + 3 * squareSize, ((screenOffset / 2)) + (tileSize / 2), tileSize, tileSize / 2, BROWN);
    DrawText("HighScore", screenOffset + (2 * lineWidth) + (3 * squareSize), screenOffset / 2 + (tileSize / 2) + lineWidth / 2, 18, LIGHTGRAY);
    DrawText(std::to_string(highScore).c_str(), screenOffset + (2 * lineWidth) + (3 * squareSize), screenOffset / 2 + (tileSize / 2) + 3 * lineWidth, 20, LIGHTGRAY);
}

void saveCurrentGame()
{

    std::ifstream fin("../bin/save_001.bin", std::ios::binary);

    if (!fin.is_open())
    {
        std::ofstream fout("../bin/save_001.bin", std::ios::binary);
        fout.write(reinterpret_cast<char *>(&currentScore), sizeof(currentScore));

        // Close binary file
        fout.close();
    }

    fin.close();
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "2048");

    SetTargetFPS(60);
    initGame();

    // Font gameFont = LoadFont("QuinqueFive.ttf");

    while (!WindowShouldClose())
    {

        if (is_game_over(totalTile))
        {
            // WaitTime(1);
            BeginDrawing();
            drawHeader();
            DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * 4) + lineWidth, (squareSize * 4) + lineWidth, BGCOLOR_SEMI_TRANS);
            DrawText("GAMEOVER", screenOffset + 60, ((screenOffset / 2) + 150) + (squareSize), 68, RED);

            if (IsKeyPressed(KEY_N))
            {
                initGame();
            }

            EndDrawing();
        }

        else
        {
            BeginDrawing();

            if (IsKeyPressed(KEY_LEFT))
            {
                last_move = totalTile;
                for (int i = 0; i < 4; i++)
                {
                    slideTilesLeft(totalTile);
                }
                sumTilesleft(totalTile);

                if (moveValid)
                {
                    generateTile();
                }
            }
            if (IsKeyPressed(KEY_RIGHT))
            {
                last_move = totalTile;
                for (int i = 0; i < 4; i++)
                {
                    slideTilesRight(totalTile);
                }
                sumTilesRight(totalTile);

                if (moveValid)
                {
                    generateTile();
                }
            }
            if (IsKeyPressed(KEY_UP))
            {
                last_move = totalTile;
                for (int i = 0; i < 4; i++)
                {
                    slideTilesUp(totalTile);
                }
                sumTilesUp(totalTile);
                if (moveValid)
                {
                    generateTile();
                }
            }
            if (IsKeyPressed(KEY_DOWN))
            {
                last_move = totalTile;
                for (int i = 0; i < 4; i++)
                {
                    slideTilesDown(totalTile);
                }
                sumTilesDown(totalTile);
                if (moveValid)
                {
                    generateTile();
                }
            }
            // for debugging
            if (IsKeyPressed(KEY_G))
            {
                generateTile();
            }
            if (IsKeyPressed(KEY_N))
            {
                initGame();
            }
            if (IsKeyPressed(KEY_U))
            {
                totalTile = last_move;
            }
            if (IsKeyPressed(KEY_S))
            {
                saveCurrentGame();
            }

            checkScore();

            ClearBackground(RAYWHITE);
            // draw header
            drawHeader();
            // draw board
            drawBoard(screenOffset, squareSize);
            // tile
            DrawTiles(totalTile);

            EndDrawing();
        }
    }

    CloseWindow();

    return 0;
}