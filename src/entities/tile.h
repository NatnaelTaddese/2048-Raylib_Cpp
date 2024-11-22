//
// Created by nat on 11/23/24.
//

#ifndef TILE_H
#define TILE_H

struct tile
{
    Vector2 absolutePosition{};
    Color tileColor{};
    int numValue = 0;
    bool isOccupied = false;
    bool isNew{};
    bool isSliding{};
    float tileAnimationProgress{};

} defaultTile;

#endif //TILE_H
