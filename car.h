#pragma once

#include "rigidBody.h"

class Car : public RigidBody
{
    public:
        Car(int id, int width, int height, float maxVel, float forceAmountPerFrame,
            float frictionCoefficient, float mass, float horizontalMultiplier, bool horizontalDir);
        virtual ~Car();
        Car(const Car& other);

        bool alive = true;  // Whether the car is alive, is used as a return value in update()
        
        void movementLogic(float deltaTime);
        bool update(std::list<RigidBody*>& rbList, Vector2& windowSize, sf::RenderWindow& window, Vector2& camPos, float deltaTime);

    private:
        float horizontalMultiplier;
        bool horizontalDir; // false = left, true = right
        int lastHitID;

        void onVerticalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camVerticalPos);
        void onHorizontalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camHorizontalPos);
        
        bool onObjectCollision(RigidBody& other);
};