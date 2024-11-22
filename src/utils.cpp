//
// Created by NAT on 11/22/2024.
//
#include <cmath>

float EaseOutElastic(const float t)
{
    if (t == 0 || t == 1)
        return t;
    constexpr float p = 0.3f;
    return pow(2, -10 * t) * sin((t - p / 4) * (2 *  M_PI) / p) + 1;
}

