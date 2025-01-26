#include "body.h"

Body::Body(int width, int height) :
    width(width), height(height)
{
    pos = new Vector2{0.0f, 0.0f};
};

Body::~Body() { delete pos; };
Body::Body(const Body& other)
{
    pos = new Vector2{other.pos->x, other.pos->y};
    width = other.width;
    height = other.height;
};

void Body::setPosition(Vector2& newPos)
{
    *pos = newPos;
}