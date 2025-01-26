#pragma once

#include "vector2.h"
#include <list>

class Body
{
    public:
        Body(int width, int height);
        virtual ~Body();
        Body(const Body& other);

        Vector2* pos;
        int width;
        int height;

        bool update(float deltaTime);
        void setPosition(Vector2& newPos);
};