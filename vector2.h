#pragma once

#include <iostream>

class Vector2
{
    public:
        float x;
        float y;

        Vector2(float x, float y);
        Vector2(float x);
        Vector2();
        ~Vector2();
        Vector2(const Vector2& other);
        
        float magnitude();
        Vector2 clampMagnitude(float mag);
        float sqrMagnitude();
        Vector2 normalized();

        void normalize();

        float dot(Vector2& other);
        float distance(Vector2& other);

        static float dot(Vector2& a, Vector2& b);
        static float distance(Vector2& a, Vector2& b);

        Vector2 operator+(const float& other) const;
        Vector2 operator-(const float& other) const;
        Vector2 operator/(const float& other) const;
        Vector2 operator*(const float& other) const;

        Vector2& operator+=(const float& other);
        Vector2& operator-=(const float& other);
        Vector2& operator*=(const float& other);
        Vector2& operator/=(const float& other);
        Vector2& operator=(const Vector2& other);
        
        Vector2 operator+(const Vector2& other) const;
        Vector2 operator-(const Vector2& other) const;
        Vector2 operator/(const Vector2& other) const;
        Vector2 operator*(const Vector2& other) const;
        
        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);
        Vector2& operator/=(const Vector2& other);
        Vector2& operator*=(const Vector2& other);

        bool operator==(const Vector2& other) const;
        bool operator!=(const Vector2& other) const;

        friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
};