#include "rigidBody.h"

constexpr float gravity = 9.80665f;

RigidBody::RigidBody(int id, int width, int height, float maxVel, float forceAmountPerFrame, float frictionCoefficient, float mass, Faction faction) :
    Body{width, height}, id(id), maxVel(maxVel), forceAmountPerFrame(forceAmountPerFrame), frictionCoefficient(frictionCoefficient), mass(mass), faction(faction)
{
    vel = new Vector2{0.0f, 0.0f};
    accel = new Vector2{0.0f, 0.0f}; 
};

RigidBody::~RigidBody()
{
    delete vel;
    delete accel;
    delete sprite;
};

RigidBody::RigidBody(const RigidBody& other) : Body(other)
{
    id = other.id; // This rigidbody should not be considered a different rigidBody
    vel = new Vector2{other.vel->x, other.vel->y};
    accel = new Vector2{other.accel->x, other.accel->y};
    maxVel  = other.maxVel;
    forceAmountPerFrame = other.forceAmountPerFrame;
    mass = other.mass;
    intangible = other.intangible;
    faction = other.faction;
    // loadSprite(*(other.sprite->getTexture())); // Can't get sprite because texture needs to be non-const
};

void RigidBody::loadSprite(sf::Texture& texture)
{
    sprite = new sf::Sprite{texture};
    sprite->setOrigin(width * 0.5f, height * 0.5f);
}

bool RigidBody::verticalCollisionDetection(const RigidBody& other, Vector2& nextPos) const
{
    return nextPos.y - height * 0.5f < other.pos->y + other.height * 0.5f &&
        nextPos.y + height * 0.5f > other.pos->y - other.height * 0.5f;
}

bool RigidBody::horizontalCollisionDetection(const RigidBody& other, Vector2& nextPos) const
{
    return nextPos.x - width * 0.5f < other.pos->x + other.width * 0.5f &&
        nextPos.x + width * 0.5f > other.pos->x - other.width * 0.5f;
}

bool RigidBody::objectCollisionDetection(const RigidBody& other, Vector2& nextPos) const
{
    return verticalCollisionDetection(other, nextPos) &&
        horizontalCollisionDetection(other, nextPos);
}

bool RigidBody::onObjectCollision(RigidBody& other) { return false; }

void RigidBody::windowDetection(Vector2& currentVel, Vector2& nextPos, Vector2& windowSize, Vector2& camPos)
{
    if (topWindowDetection(nextPos, 0.0f + camPos.y)) { onVerticalWindowHit(currentVel, nextPos, 0.0f, camPos.y); }
    else if (bottomWindowDetection(nextPos, windowSize.y + camPos.y)) { onVerticalWindowHit(currentVel, nextPos, windowSize.y, camPos.y); }
    if (leftWindowDetection(nextPos, 0.0f + camPos.x)) { onHorizontalWindowHit(currentVel, nextPos, 0.0f, camPos.x); }
    else if (rightWindowDetection(nextPos, windowSize.x + camPos.x)) { onHorizontalWindowHit(currentVel, nextPos, windowSize.x, camPos.x); }
}

bool RigidBody::topWindowDetection(Vector2& nextPos, float windowTopPos) const { return nextPos.y - height * 0.5f < windowTopPos; }
bool RigidBody::bottomWindowDetection(Vector2& nextPos, float windowBottomPos) const { return nextPos.y + height * 0.5f > windowBottomPos; }
bool RigidBody::leftWindowDetection(Vector2& nextPos, float windowLeftPos) const { return nextPos.x - width * 0.5f < windowLeftPos; }
bool RigidBody::rightWindowDetection(Vector2& nextPos, float windowRightPos) const { return nextPos.x + width * 0.5f > windowRightPos; }

void RigidBody::onVerticalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camVerticalPos) {}
void RigidBody::onHorizontalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camHorizontalPos) {}

void RigidBody::addForce(Vector2& force, ForceMode fMode, float deltaTime)
{
    switch (fMode)
    {
        case ForceMode::FORCE:
                *accel = force * deltaTime / mass;
            break;
        case ForceMode::ACCELERATION:
                *accel = force * deltaTime;
            break;
        case ForceMode::IMPULSE:
                *accel = force / mass;
            break;
        case ForceMode::VELOCITYCHANGE:
                *accel = force;
            break;
    }
    *vel += *accel;
}

void RigidBody::updateNextPos(std::list<RigidBody*>& rbList, Vector2& windowSize, Vector2& camPos, float deltaTime)
{
    // If moving, calculate friction
    if (vel->magnitude() > 0.0f)
    {
        Vector2 friction{gravity * mass * frictionCoefficient};
        // Friction is the opposite direction of velocity
        friction *= vel->normalized() * -1;

        Vector2 forceApplied = *accel * mass;
        *accel = (forceApplied + friction) / mass;

        // std::cout << *accel << std::endl; 
    }

    // Get the delta velocity
    Vector2 deltaVel = *accel * deltaTime;
    Vector2 newVel;

    // If x or y reach 0, let it stay 0 and reset acceleration
    if (vel->x > 0.0f && -vel->x >= deltaVel.x || 
        vel->x < 0.0f && -vel->x <= deltaVel.x) { newVel.x, accel->x = 0.0f; }
    else { newVel.x = vel->x + deltaVel.x; }

    if (vel->y > 0.0f && -vel->y >= deltaVel.y || 
        vel->y < 0.0f && -vel->y <= deltaVel.y) { newVel.y, accel->y = 0.0f; }
    else { newVel.y = vel->y + deltaVel.y; }

    newVel = newVel.magnitude() <= maxVel ? newVel :
        newVel.clampMagnitude(maxVel);

    Vector2 newPos = *pos + (*vel + newVel) / 2.0f * deltaTime;

    // std::cout << newVel << std::endl; 

    // Check object collision
    bool stopMovement = false;
    
    for (RigidBody* rbObject : rbList)
    {
        // Skip self
        if (rbObject == this) { continue; }

        // // Skip own faction
        // if (rbObject->faction == faction) { continue; }

        if (objectCollisionDetection(*rbObject, newPos))
        {
            rbObject->onObjectCollision(*this);
            bool stopping = onObjectCollision(*rbObject);
            
            stopMovement = stopMovement || stopping;
        }
    }

    // Check window detection
    windowDetection(newVel, newPos, windowSize, camPos);

    if (!stopMovement) { *vel = newVel; *pos = newPos; }
    else { *vel = {0.0f, 0.0f}; }
}