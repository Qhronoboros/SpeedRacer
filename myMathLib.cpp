#include <iostream>
#include "myMathLib.h"

namespace MyMathLib
{
    int abs(int value) { return value < 0 ? -value : value; }
    float abs(float value) { return value < 0.0f ? -value : value; }
    Vector2 abs(Vector2 vec2) { return {abs(vec2.x), abs(vec2.y)}; }

    int min(int a, int b) { return a <= b ? a : b; }
    float min(float a, float b) { return a <= b ? a : b; }
    Vector2 min(Vector2 vec2, float b) { return {min(vec2.x, b), min(vec2.y, b)}; }

    int max(int a, int b) { return a >= b ? a : b; }
    float max(float a, float b) { return a >= b ? a : b; }
    Vector2 max(Vector2 vec2, float b) { return {max(vec2.x, b), max(vec2.y, b)}; }

    int clamp(int value, int minimum, int maximum)
    {
        return min(max(value, minimum), maximum);
    }

    float clamp(float value, float minimum, float maximum)
    {
        return min(max(value, minimum), maximum);
    }

    Vector2 clamp(Vector2 vec2, float minimum, float maximum)
    {
        return {clamp(vec2.x, minimum, maximum), clamp(vec2.y, minimum, maximum)};
    }

    // Float Modulo
    float fmod(float numer, float denom)
    {
        return numer - trunc(numer / denom) * denom; 
    }

    float floor(float value)
    {
        float truncated = trunc(value);
        float remainder = value - truncated;
        return value < 0.0f && remainder < 0.0f ? --truncated : truncated;
    }

    float ceil(float value)
    {
        float truncated = trunc(value);
        float remainder = value - truncated;
        return value > 0.0f && remainder > 0.0f ? ++truncated : truncated;
    }

    float trunc(float value) { return float(int(value)); }

    // ! Rounding may not be accurate if given decimals value is too high
    // * Reason: float type is too small, could be adjusted to double if needed
    float round(float value, int decimals)
    {
        float decimalShift = pow(10.0f, decimals);
        float shiftedValue = value * decimalShift;
        float truncated = trunc(shiftedValue);
        float remainder = shiftedValue - truncated;

        if (remainder <= -0.5f) { truncated -= 1.0f; }
        else if (remainder >= 0.5f) { truncated += 1.0f; }

        return truncated / decimalShift;
    }

    // Power
    // ! only whole numbers for exponent are allowed
    float pow(float base, int exponent)
    {
        float result = 1.0f;
        if (exponent > 0) { for (int i = 0; i < exponent; i++) { result *= base; } }
        else if (exponent < 0) { for (int i = 0; i > exponent; i--) { result /= base; } }

        return result;
    }

    // Square Root
    // ! negative radicants are not allowed, and index can not be negative or 0
    float sqrt(float radicant, int index)
    {
        if (radicant < 0) { std::cerr << "sqrt: given radicant can't be negative." << std::endl; exit(-1); }
        if (index <= 0) { std::cerr << "sqrt: given index can't be negative or 0." << std::endl; exit(-1); }

        float estimate = 0.0f;
        float increment = 1.0f;
        bool found = false;

        while (!found)
        {
            if (pow(estimate, index + 1) == radicant) { found = true; } // Found the solution

            while (pow(estimate, index + 1) <= radicant)
            {
                float estimateOld = estimate;
                estimate += increment;
                if (estimateOld == estimate) { found = true; break; } // If too many decimals for type float
            }
            estimate -= increment;

            increment *= 0.1f;
        }

        return estimate;
    }

    // Greatest Common Divisor using the Euclidean Algorithm (Recursion)
    // ! a and b can not be negative
    int gcd(int a, int b)
    {
        if (a < 0 || b < 0) { std::cerr << "gcd: given values can't be negative." << std::endl; exit(-1); }
        
        int currentA = max(a, b); int currentB = min(a, b);

        if (currentB == 0) { return currentA; }
        else { return gcd(currentB, currentA % currentB); }
    }

    float exp(int exponent) { return pow((float)M_E, exponent); }

    // Returns the exponent of the given base and value
    int log(float base, float value)
    {
        int potentialExp = 0;
        float calculatedBase = base;

        while(calculatedBase >= base) { calculatedBase = sqrt(value, ++potentialExp); }
        
        return --potentialExp;
    }
}