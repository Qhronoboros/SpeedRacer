#include "car.h"

Car::Car(int id, int width, int height, float maxVel, float forceAmountPerFrame, float frictionCoefficient,
    float mass, float horizontalMultiplier, bool horizontalDir) :
    RigidBody{id, width, height, maxVel, forceAmountPerFrame, frictionCoefficient, mass, Faction::CAR},
    horizontalMultiplier(horizontalMultiplier), horizontalDir(horizontalDir) {};

Car::~Car() = default;
Car::Car(const Car& other) : RigidBody(other)
{
    alive = other.alive;
    horizontalMultiplier = other.horizontalMultiplier;
    horizontalDir = other.horizontalDir;
    lastHitID = other.lastHitID;
};

void Car::movementLogic(float deltaTime)
{
    addForce(Vector2{forceAmountPerFrame * horizontalMultiplier * (int(horizontalDir)*2-1), forceAmountPerFrame}, ForceMode::ACCELERATION, deltaTime);
}

bool Car::update(std::list<RigidBody*>& rbList, Vector2& windowSize, sf::RenderWindow& window, Vector2& camPos, float deltaTime)
{
    movementLogic(deltaTime);
    updateNextPos(rbList, windowSize, camPos, deltaTime);

    Vector2 screenSpace = *pos - camPos;
    sprite->setPosition(screenSpace.x, screenSpace.y);

    window.draw(*sprite);

    return alive;
}

// When car is outside the screen on the bottom of the window, delete car
void Car::onVerticalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camVerticalPos)
{
    if (windowPos != 0.0f && nextPos.y - camVerticalPos - windowPos > height * 0.5f) { alive = false; }
}

// Bounce car when hitting left or right side of the window
void Car::onHorizontalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camHorizontalPos)
{
    if (windowPos <= 0.0f) // Hit left side of the game window
    {
        currentVel.x = MyMathLib::abs(currentVel.x);
        horizontalDir = true;
    }
    else // Hit right side of the game window
    {
        currentVel.x = MyMathLib::abs(currentVel.x) * -1;
        horizontalDir = false;
    }
}

bool Car::onObjectCollision(RigidBody& other)
{
    // If other object is a car and the other car was not lastHit, set velocity to move away from other car
    if (other.faction == Faction::CAR && lastHitID != other.id)
    {
        lastHitID = other.id;

        if (pos->x < other.pos->x)
        {
            vel->x = MyMathLib::abs(vel->x) * -1;
            horizontalDir = false;
        }
        else
        {
            vel->x = MyMathLib::abs(vel->x);
            horizontalDir = true;
        }
        return true;
    }

    return false;
}