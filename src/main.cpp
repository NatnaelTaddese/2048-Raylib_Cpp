/*******************************************************************************************
 *
 *   raylib [core]
 *
 ********************************************************************************************/

#include "raylib.h"
#include <array>
#include <string>

// #include <vector>
#include <iostream>
#include <fstream>

#include "themes/default_theme.h"
#include "entities/level.h"
#include "entities/grid.h"
#include "entities/tile.h"
#include "utils/ease_out_elastic.cpp"

// GLOBAL VARIABLES
constexpr int screenOffset = 0; // original: 50
constexpr int squareSize = 120;
constexpr int lineWidth = 10;
constexpr int tileSize = squareSize - lineWidth;
int screenWidth = 490;  // original: 600
int screenHeight = 640; // original: 750
int fixFontPosition;
int fixFontSize;
int currentScore;
int highScore;
int ROWS = 4;
int COLS = ROWS;
double lastUpdateTime = 0;
bool moveValid = false;
bool splashScreen = true;
bool isFirstTime = true;
bool showTutorial = true;
bool pause = false;
Color tileColor;
Color numColor;



struct user
{
    std::string userName;
    int highScore;
};

// std::vector<user> users;

// vector that stores all the squareTiles
std::array<std::array<tile, 4>, 4> totalTile4X4;
std::array<std::array<tile, 5>, 5> totalTile5X5;
std::array<std::array<tile, 6>, 6> totalTile6X6;

std::array<std::array<tile, 4>, 4> last_move4X4;
std::array<std::array<tile, 5>, 5> last_move5X5;
std::array<std::array<tile, 6>, 6> last_move6X6;

float lerp(const float a, const float b, const float t)
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

// Generate tile
template <class T>
void generateTile(T &totalTile)
{
    int x, y, r_num;

    do
    {
        x = rand() % ROWS;
        y = rand() % COLS;
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

template <class T>
bool is_game_over(T &totalTile)
{
    // Check if there are any empty tiles
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (totalTile[i][j].numValue == 0)
            {
                return false;
            }
        }
    }

    // Check if there are any adjacent tiles with the same value
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if ((i < (ROWS - 1) && totalTile[i][j].numValue == totalTile[i + 1][j].numValue) ||
                (j < (COLS - 1) && totalTile[i][j].numValue == totalTile[i][j + 1].numValue))
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
template <class T>
inline void DrawTiles(T &totalTile)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
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
                tileColor = totalTile[i][j].numValue == 2 ? TILE_COLOR_2 : totalTile[i][j].numValue == 4 ? TILE_COLOR_4
                                                                     : totalTile[i][j].numValue == 8   ? TILE_COLOR_8
                                                                     : totalTile[i][j].numValue == 16  ? TILE_COLOR_16
                                                                     : totalTile[i][j].numValue == 32  ? TILE_COLOR_32
                                                                     : totalTile[i][j].numValue == 64  ? TILE_COLOR_64
                                                                     : totalTile[i][j].numValue == 128 ? TILE_COLOR_128
                                                                     : totalTile[i][j].numValue == 256 ? TILE_COLOR_256
                                                                                                       : TILE_COLOR_128;

                numColor = totalTile[i][j].numValue < 16 ? NUM_COLOR_DARK : NUM_COLOR;

                if (totalTile[i][j].isNew)
                {
                    // Slower progression
                    totalTile[i][j].tileAnimationProgress += 0.02f; // Reduced speed

                    // Scale factor using smoother easing function
                    float scale = EaseOutElastic(totalTile[i][j].tileAnimationProgress);

                    float scaledTileSize = tileSize * scale;

                    // Center the scaled tile
                    float offsetX = (tileSize - scaledTileSize) / 2;
                    float offsetY = (tileSize - scaledTileSize) / 2;

                    DrawRectangle(
                        totalTile[i][j].absolutePosition.x + offsetX,
                        totalTile[i][j].absolutePosition.y + offsetY,
                        scaledTileSize,
                        scaledTileSize,
                        tileColor);

                    // Draw the tile value
                    DrawText(std::to_string(totalTile[i][j].numValue).c_str(),
                             (totalTile[i][j].absolutePosition.x + offsetX + (scaledTileSize / 2) - 10) - fixFontPosition,
                             (totalTile[i][j].absolutePosition.y + offsetY + (scaledTileSize / 2) - 30),
                             60 - fixFontSize,
                             numColor);

                    // End animation
                    if (totalTile[i][j].tileAnimationProgress >= 1.0f)
                    {
                        totalTile[i][j].isNew = false;
                    }
                }
                else
                {
                    DrawRectangle(totalTile[i][j].absolutePosition.x, totalTile[i][j].absolutePosition.y, tileSize, tileSize, tileColor);
                    DrawText(std::to_string(totalTile[i][j].numValue).c_str(),
                             (totalTile[i][j].absolutePosition.x + (tileSize / 2) - 10) - fixFontPosition,
                             (totalTile[i][j].absolutePosition.y + (tileSize / 2) - 30),
                             60 - fixFontSize,
                             numColor);
                }
            }
        }
    }
}


// slide tiles according to the direction applied
template <class T>
void slideTilesLeft(T &totalTile)
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 1; j < COLS; j++)
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
template <class T>
void slideTilesRight(T &totalTile)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = (COLS - 2); j >= 0; j--)
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
template <class T>
void slideTilesUp(T &totalTile)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 1; j < COLS; j++)
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
template <class T>
void slideTilesDown(T &totalTile)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = (COLS - 2); j >= 0; j--)
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
template <class T>
void sumTilesleft(T &totalTile)
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 1; j < COLS; j++)
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
template <class T>
void sumTilesRight(T &totalTile)
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = (COLS - 2); j >= 0; j--)
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
template <class T>
void sumTilesUp(T &totalTile)
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 1; j < COLS; j++)
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
template <class T>
void sumTilesDown(T &totalTile)
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = (COLS - 2); j >= 0; j--)
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
void drawBoard(const int screenOffset, const int squareSize)
{
    // grid background rectangle
    DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * ROWS), (squareSize * COLS), BG_COLOR);

    // horizontal grid
    for (int i = 0; i <= ROWS; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset, ((screenOffset / 2) + 150) + spacing, (squareSize * ROWS) + lineWidth, lineWidth, GRID_COLOR);
    }
    // vertical grid
    for (int i = 0; i <= COLS; i++)
    {
        int spacing = squareSize * i;
        DrawRectangle(screenOffset + spacing, ((screenOffset / 2) + 150), lineWidth, (squareSize * COLS), GRID_COLOR);
    }
}

template <class T>
void initGame(T &totalTile)
{
    // initializing defalut values before running game

    currentScore = 0;
    // fill the array with default tile
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            totalTile[i][j] = defaultTile;
        }
    }
    generateTile(totalTile);
    generateTile(totalTile);
    readScore();
    readScore();
}

void drawHeader()
{
    // header text defaults
    DrawText("2048", screenOffset, screenOffset / 2, 62, LIGHTGRAY);

    // score board
    DrawRectangle(screenOffset + lineWidth + (ROWS - 1) * squareSize, (screenOffset / 2), tileSize, tileSize / 2, BROWN);
    DrawText("Score", screenOffset + (2 * lineWidth) + ((ROWS - 1) * squareSize), (screenOffset / 2) + lineWidth, 20, LIGHTGRAY);
    DrawText(std::to_string(currentScore).c_str(), screenOffset + (2 * lineWidth) + ((ROWS - 1) * squareSize), (screenOffset / 2) + 30, 20, LIGHTGRAY);

    DrawRectangle(screenOffset + lineWidth + (ROWS - 1) * squareSize, ((screenOffset / 2)) + (tileSize / 2) + lineWidth, tileSize, tileSize / 2, BROWN);
    DrawText("HighScore", screenOffset + (2 * lineWidth) + ((ROWS - 1) * squareSize), screenOffset / 2 + (tileSize / 2) + lineWidth + 3, 18, LIGHTGRAY);
    DrawText(std::to_string(highScore).c_str(), screenOffset + (2 * lineWidth) + ((ROWS - 1) * squareSize), screenOffset / 2 + (tileSize / 2) + 3 * lineWidth + 3, 20, LIGHTGRAY);
}

template <class T>
void saveCurrentGame(T &totalTile)
{

    std::ofstream fout("../bin/save_001.txt");

    fout << level << " ";
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fout << totalTile[i][j].absolutePosition.x << " ";
            fout << totalTile[i][j].absolutePosition.y << " ";
            fout << totalTile[i][j].numValue << " ";
            fout << totalTile[i][j].isOccupied << " ";
            fout << totalTile[i][j].isNew << " ";
            fout << totalTile[i][j].tileAnimationProgress << " ";
        }
    }
    // Close file
    fout.close();
}

void loadGame()
{
    std::ifstream fin("../bin/save_001.txt");

    int s_level;
    fin >> s_level;

    switch (s_level)
    {
    case 0:
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                fin >> totalTile6X6[i][j].absolutePosition.x;
                fin >> totalTile6X6[i][j].absolutePosition.y;
                fin >> totalTile6X6[i][j].numValue;
                fin >> totalTile6X6[i][j].isOccupied;
                fin >> totalTile6X6[i][j].isNew;
                fin >> totalTile6X6[i][j].tileAnimationProgress;
            }
        }
        level = easy;
        ROWS = COLS = 6;
        InitWindow(screenWidth + 220, screenHeight + 160, "2048");
        isFirstTime = false;
        splashScreen = false;

        break;
    case 1:

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                fin >> totalTile5X5[i][j].absolutePosition.x;
                fin >> totalTile5X5[i][j].absolutePosition.y;
                fin >> totalTile5X5[i][j].numValue;
                fin >> totalTile5X5[i][j].isOccupied;
                fin >> totalTile5X5[i][j].isNew;
                fin >> totalTile5X5[i][j].tileAnimationProgress;
            }
        }
        level = medium;
        InitWindow(screenWidth + 120, screenHeight + 50, "2048");
        initGame(totalTile5X5);
        isFirstTime = false;
        splashScreen = false;

        break;
    case 2:

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                fin >> totalTile4X4[i][j].absolutePosition.x;
                fin >> totalTile4X4[i][j].absolutePosition.y;
                fin >> totalTile4X4[i][j].numValue;
                fin >> totalTile4X4[i][j].isOccupied;
                fin >> totalTile4X4[i][j].isNew;
                fin >> totalTile4X4[i][j].tileAnimationProgress;
            }
        }
        level = easy;
        COLS = ROWS = 4;
        isFirstTime = false;
        splashScreen = false;

        break;
    default:
        std::cout << "error loading game " << level << std::endl;
        break;
    }

    fin.close();
}

void drawModeSelector()
{
    checkScore();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawHeader();
    DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * 4) + lineWidth, (squareSize * 4) + lineWidth, BG_COLOR);
    DrawText("SELECT MODE", screenOffset + 60, ((screenOffset / 2) + 100) + (squareSize), 48, BROWN);
    DrawText("Press 1 for easy mode (6X6)", screenOffset + 45, ((screenOffset / 2) + 250) + (squareSize), 28, LIME);
    DrawText("Press 2 for medium mode(5X5)", screenOffset + 40, ((screenOffset / 2) + 300) + (squareSize), 28, BROWN);
    DrawText("Press 3 for Hard mode(4X4)", screenOffset + 45, ((screenOffset / 2) + 350) + (squareSize), 28, RED);

    if (IsKeyPressed(KEY_ONE))
    {
        level = hard;
        COLS = ROWS = 6;
        InitWindow(screenWidth + 240, screenHeight + 240, "2048");
        initGame(totalTile6X6);
        isFirstTime = false;
    }
    if (IsKeyPressed(KEY_TWO))
    {
        level = medium;
        COLS = ROWS = 5;
        InitWindow(screenWidth + 120, screenHeight + 120, "2048");
        initGame(totalTile5X5);
        isFirstTime = false;
    }
    if (IsKeyPressed(KEY_THREE))
    {

        level = easy;
        COLS = ROWS = 4;
        initGame(totalTile4X4);
        isFirstTime = false;
    }

    EndDrawing();
}
void drawMenu()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // drawHeader();
    DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * 4) + lineWidth, (squareSize * 4) + lineWidth, BG_COLOR);
    DrawText("Tutorial", screenOffset + 60, ((screenOffset / 2) + 150) + (squareSize), 68, BROWN);
    DrawText("(^)", screenOffset + 423, ((screenOffset / 2) + 225) + (squareSize), 28, LIME);

    DrawText("Move Tiles with arrow keys (<- , ->)", screenOffset + 10, ((screenOffset / 2) + 250) + (squareSize), 28, LIME);
    DrawText("Press N for a NewGame", screenOffset + 40, ((screenOffset / 2) + 300) + (squareSize), 28, BROWN);
    DrawText("Press L to load a Game", screenOffset + 40, ((screenOffset / 2) + 350) + (squareSize), 28, BROWN);
    DrawText("Press U to Undo last Move", screenOffset + 40, ((screenOffset / 2) + 400) + (squareSize), 28, BROWN);

    if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ENTER))
    {
        splashScreen = false;
        EndDrawing();
        drawModeSelector();
    }

    if (IsKeyPressed(KEY_L))
    {
        EndDrawing();
        loadGame();
    }

    EndDrawing();
}

template <class T>
void update(T &totalTile, T &last_move)
{

    if (is_game_over(totalTile))
    {
        // WaitTime(1);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawHeader();
        DrawRectangle(screenOffset, (screenOffset / 2) + 150, (squareSize * ROWS) + lineWidth, (squareSize * COLS) + lineWidth, BG_COLOR_SEMI_TRANSPARENT);
        DrawText("GAME OVER", screenOffset + 60, ((screenOffset / 2) + 150) + (squareSize), 68, RED);
        DrawText("Press R to Retry", screenOffset + 40, ((screenOffset / 2) + 300) + (squareSize), 28, BROWN);
        DrawText("Press N for a NewGame", screenOffset + 45, ((screenOffset / 2) + 350) + (squareSize), 28, LIME);

        if (IsKeyPressed(KEY_R))
        {
            initGame(totalTile);
        }

        if (IsKeyPressed(KEY_N))
        {
            isFirstTime = true;
        }

        EndDrawing();
    }

    else
    {
        BeginDrawing();

        if (IsKeyPressed(KEY_LEFT))
        {
            last_move = totalTile;
            for (int i = 0; i < ROWS; i++)
            {
                slideTilesLeft(totalTile);
            }
            sumTilesleft(totalTile);

            if (moveValid)
            {
                generateTile(totalTile);
            }
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            last_move = totalTile;
            for (int i = 0; i < ROWS; i++)
            {
                slideTilesRight(totalTile);
            }
            sumTilesRight(totalTile);

            if (moveValid)
            {
                generateTile(totalTile);
            }
        }
        if (IsKeyPressed(KEY_UP))
        {
            last_move = totalTile;
            for (int i = 0; i < ROWS; i++)
            {
                slideTilesUp(totalTile);
            }
            sumTilesUp(totalTile);
            if (moveValid)
            {
                generateTile(totalTile);
            }
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            last_move = totalTile;
            for (int i = 0; i < ROWS; i++)
            {
                slideTilesDown(totalTile);
            }
            sumTilesDown(totalTile);
            if (moveValid)
            {
                generateTile(totalTile);
            }
        }
        // for debugging
        if (IsKeyPressed(KEY_G))
        {
            generateTile(totalTile);
        }
        if (IsKeyPressed(KEY_N))
        {
            initGame(totalTile);
        }
        if (IsKeyPressed(KEY_U))
        {
            totalTile = last_move;
        }

        saveCurrentGame(totalTile);

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

int main()
{
    InitWindow(screenWidth, screenHeight, "2048");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (splashScreen)
        {
            drawMenu();
        }

        else if (isFirstTime)
        {
            drawModeSelector();
        }

        else
        {

            switch (level)
            {
            case hard:
                update(totalTile6X6, last_move6X6);
                break;
            case medium:
                update(totalTile5X5, last_move5X5);
                break;
            case easy:
                update(totalTile4X4, last_move4X4);
                break;

            default:
                break;
            }
        }
    }

    CloseWindow();

    return 0;
}