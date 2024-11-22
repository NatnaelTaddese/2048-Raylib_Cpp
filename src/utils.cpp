//
// Created by NAT on 11/22/2024.
//
#include <cmath>

float EaseOutElastic(float t)
{
    if (t == 0 || t == 1)
        return t;
    float p = 0.5f; // Increase this for a slower "elastic" effect
    return pow(2, -10 * t) * sin((t - p / 4) * (2 * M_PI) / p) + 1;
}


