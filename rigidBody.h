#pragma once

#include "body.h"
#include "myMathLib.h"
#include <SFML/Graphics.hpp>

// Force            -   v += f * dt / m     -   time and mass
// Acceleration     -   v += f * dt         -   time
// Impulse          -   v += f / m          -   mass
// VelocityChange   -   v += f              -

enum class ForceMode { FORCE, ACCELERATION, IMPULSE, VELOCITYCHANGE };
enum class Faction { PLAYER, CAR };

class RigidBody : public Body
{
    public:
        RigidBody(int id, int width, int height, float maxVel, float forceAmountPerFrame,
            float frictionCoefficient, float mass, Faction faction);
        virtual ~RigidBody();
        RigidBody(const RigidBody& other);

        int id;
        Vector2* vel;
        float mass;
        bool intangible = false;
        Faction faction;
        sf::Sprite* sprite;

        virtual bool update(std::list<RigidBody*>& rbList, Vector2& windowSize, sf::RenderWindow& window, Vector2& camPos, float deltaTime) = 0;
        void loadSprite(sf::Texture& texture);
        void addForce(Vector2& force, ForceMode fMode, float deltaTime);

    protected:
        Vector2* accel;
        float maxVel;
        float forceAmountPerFrame;
        float frictionCoefficient; // Between 0.0f and 1.0f

        void updateNextPos(std::list<RigidBody*>& rbList, Vector2& windowSize, Vector2& camPos, float deltaTime);

        bool verticalCollisionDetection(const RigidBody& other, Vector2& nextPos) const;
        bool horizontalCollisionDetection(const RigidBody& other, Vector2& nextPos) const;
        bool objectCollisionDetection(const RigidBody& other, Vector2& nextPos) const;
        virtual bool onObjectCollision(RigidBody& other);
        
        bool topWindowDetection(Vector2& nextPos, float windowTopPos) const;
        bool bottomWindowDetection(Vector2& nextPos, float windowBottomPos) const;
        bool leftWindowDetection(Vector2& nextPos, float windowLeftPos) const;
        bool rightWindowDetection(Vector2& nextPos, float windowRightPos) const;
        
        void windowDetection(Vector2& currentVel, Vector2& nextPos, Vector2& windowSize, Vector2& camPos);
        virtual void onVerticalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camVerticalPos);
        virtual void onHorizontalWindowHit(Vector2& currentVel, Vector2& nextPos, float windowPos, float camHorizontalPos);
};