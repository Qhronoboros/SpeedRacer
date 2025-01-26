#include "player.h"

Player::Player(int id, int width, int height, float maxVel, float forceAmountPerFrame,
    float frictionCoefficient, float mass, int maxHealth, float maxIntangibleTime) :
        RigidBody{id ,width, height, maxVel, forceAmountPerFrame, frictionCoefficient, mass, Faction::PLAYER},
        health(maxHealth) ,maxHealth(maxHealth), maxIntangibleTime(maxIntangibleTime),
        intangibleTimer(maxIntangibleTime) {};

Player::~Player() = default;
Player::Player(const Player& other) : RigidBody{other}
{
    health = other.health;
    maxHealth = other.maxHealth;
    hit = other.hit;
    maxIntangibleTime = other.maxIntangibleTime;
    intangibleTimer = other.intangibleTimer;
};

void Player::movementLogic(bool left, bool right, bool up, bool down, float deltaTime)
{
    float horizontalForce = left ? -forceAmountPerFrame : right ? forceAmountPerFrame : 0.0f;
    // Prevent player from moving backwards by checking if the velocity is negative
    // Added an additional multiplier for decreasing movement, so the player can slow down quickly when needed
    float verticalForce = up ? -forceAmountPerFrame : down && vel->y < 0.0f ? forceAmountPerFrame * 3.0f : 0.0f;

    if (horizontalForce != 0.0f || verticalForce != 0.0f)
    {
        addForce(Vector2{horizontalForce, verticalForce}, ForceMode::ACCELERATION, deltaTime);
    }
}

bool Player::update(std::list<RigidBody*>& rbList, Vector2& windowSize, sf::RenderWindow& window, Vector2& camPos, float deltaTime)
{
    if (intangible)
    {
        intangibleTimer += deltaTime;
        if (intangibleTimer >= maxIntangibleTime) { intangible = false; }
    }

    updateNextPos(rbList, windowSize, camPos, deltaTime);

    Vector2 screenSpace = *pos - camPos;
    sprite->setPosition(screenSpace.x, screenSpace.y);

    // Alternate drawing the player when intangible
    if (this->intangible && (int)(this->intangibleTimer * 10.0f) % 2 != 0) { }
    else { window.draw(*sprite); }

    return hit;
}

// Keep the player within the window horizontally
void Player::onHorizontalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camHorizontalPos)
{
    currentVel.x = 0.0f;
    float halfWidth = width * 0.5f;
    nextPos.x = windowPos <= 0.0f ? windowPos + halfWidth : windowPos - halfWidth ;
}

bool Player::onObjectCollision(RigidBody& other)
{
    // If other object is a car and player is not intangible, damage player
    if (other.faction == Faction::CAR && !intangible)
    {
        hit = true;
        health--;
        *vel = Vector2{};

        intangibleTimer = 0.0f;
        intangible = true;

        return true;
    }

    return false;
}