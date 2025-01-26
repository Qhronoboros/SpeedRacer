#include "vector2.h"
#include "myMathLib.h"

Vector2::Vector2(float x, float y) : x(x), y(y) {};
Vector2::Vector2(float x) : x(x), y(x) {};
Vector2::Vector2() : x(0.0f), y(0.0f) {};
Vector2::~Vector2() = default;
Vector2::Vector2(const Vector2& other) = default;

float Vector2::magnitude()
{
    return MyMathLib::sqrt(x * x + y * y);
}

Vector2 Vector2::clampMagnitude(float mag) { return normalized() * mag; }

float Vector2::sqrMagnitude() { return x * x + y * y; }

Vector2 Vector2::normalized()
{
    float mag = magnitude();
    if (mag == 0.0f) { mag = 1.0f; }
    return {x / mag, y / mag};
}

void Vector2::normalize()
{
    float mag = magnitude();
    if (mag == 0.0f) { mag = 1.0f; }
    x /= mag, y /= mag;
}

float Vector2::dot(Vector2& other) { return Vector2::dot(*this, other); }
float Vector2::distance(Vector2& other) { return Vector2::distance(*this, other); }

float Vector2::dot(Vector2& a, Vector2& b) { return a.x * b.x + a.y * b.y; }
float Vector2::distance(Vector2& a, Vector2& b) { return (b - a).magnitude(); }

Vector2 Vector2::operator+(const float& other) const { return {x + other, y + other}; };
Vector2 Vector2::operator-(const float& other) const { return {x - other, y - other}; };
Vector2 Vector2::operator/(const float& other) const { return {x / other, y / other}; };
Vector2 Vector2::operator*(const float& other) const { return {x * other, y * other}; };

Vector2& Vector2::operator+=(const float& other)
{
    x += other, y += other;
    return *this;
};

Vector2& Vector2::operator-=(const float& other)
{
    x -= other, y -= other;
    return *this;
};

Vector2& Vector2::operator*=(const float& other)
{
    x *= other, y *= other;
    return *this;
};

Vector2& Vector2::operator/=(const float& other)
{
    x /= other, y /= other;
    return *this;
};

Vector2& Vector2::operator=(const Vector2& other)
{
    if (*this != other) { x = other.x; y = other.y; }
    return *this;
};

Vector2 Vector2::operator+(const Vector2& other) const { return {x + other.x, y + other.y}; };
Vector2 Vector2::operator-(const Vector2& other) const { return {x - other.x, y - other.y}; };
Vector2 Vector2::operator/(const Vector2& other) const { return {x / other.x, y / other.y}; };
Vector2 Vector2::operator*(const Vector2& other) const { return {x * other.x, y * other.y}; };

Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x, y += other.y;
    return *this;
};

Vector2& Vector2::operator-=(const Vector2& other)
{
    x -= other.x, y -= other.y;
    return *this;
};

Vector2& Vector2::operator/=(const Vector2& other)
{
    x /= other.x, y /= other.y;
    return *this;
};

Vector2& Vector2::operator*=(const Vector2& other)
{
    x *= other.x, y *= other.y;
    return *this;
};

bool Vector2::operator==(const Vector2& other) const { return x == other.x && y == other.y; };
bool Vector2::operator!=(const Vector2& other) const { return x != other.x || y != other.y; };

std::ostream& operator<<(std::ostream& os, const Vector2& vec2)
{
    return os << "vector2 [" << vec2.x << ", " << vec2.y << "]";
};
