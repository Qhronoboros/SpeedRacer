#pragma once

#include "rigidBody.h"

class Player : public RigidBody
{
    public:
        Player(int id, int width, int height, float maxVel, float forceAmountPerFrame,
            float frictionCoefficient, float mass, int maxHealth, float maxIntangibleTime);
        virtual ~Player();
        Player(const Player& other);

        int health;
        int maxHealth;
        bool hit = false;   // Whether the player has been hit, is used as a return value in update()

        void movementLogic(bool left, bool right, bool up, bool down, float deltaTime);
        bool update(std::list<RigidBody*>& rbList, Vector2& windowSize, sf::RenderWindow& window, Vector2& camPos, float deltaTime);

    private:
        float maxIntangibleTime; // How long the player can be intangible for in seconds
        float intangibleTimer;

        void onHorizontalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camHorizontalPos);
        bool onObjectCollision(RigidBody& other);
};