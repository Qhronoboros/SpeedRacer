#pragma once

#include "vector2.h"

namespace MyMathLib
{
    constexpr double M_PI = 3.14159265358979323846;
    constexpr double M_E = 2.71828182845904523536;

    int abs(int value);
    float abs(float value);
    Vector2 abs(Vector2 vec2);

    int min(int a, int b);
    float min(float a, float b);
    Vector2 min(Vector2 vec2, float b);

    int max(int a, int b);
    float max(float a, float b);
    Vector2 max(Vector2 vec2, float b);

    int clamp(int value, int minimum, int maximum);
    float clamp(float value, float minimum, float maximum);
    Vector2 clamp(Vector2 vec2, float minimum, float maximum);

    float fmod(float numer, float denom);
    float floor(float value);
    float ceil(float value);
    float trunc(float value);

    float round(float value, int decimals);

    float pow(float base, int exp);
    float sqrt(float radicant, int index=1);
    int gcd(int a, int b);

    float exp(float value);
    int log(float base, float value);
}