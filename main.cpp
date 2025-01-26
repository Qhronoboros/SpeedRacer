// Made by Francisco Ngo
// Art Assets: Kenney (https://kenney.nl/)
// Font: https://www.dafont.com/super-cartoon.font

// * Move the player character using WASD

#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>

#include "myMathLib.h"
#include "vector2.h"
#include "rigidBody.h"
#include "player.h"
#include "car.h"

using namespace std;

// The size of the game window, needs to be constructed in main
Vector2 windowSize;

// List of all rigidbodies within the game
// * Pointer because parent class is abstract
list<RigidBody*> rbList;

// ID for identifying rigidBodies
int idCounter = 0;

// Values that get added to the player's score for specific conditions
float scoreForTravel = 0.01f;
float scoreForDodging = 10.0f;

// The score the player needs to obtain to win the game
float winCondition = 1000.0f;

// The amount of distance it takes to increase the maximum car amount (Difficulty increase over time)
float diffIncrDistance = 5000.0f;

// The position of the camera, is used to convert world space to screen space
Vector2 cameraPosition{};
// The vertical offset of the camera from the player
float cameraVerticalOffset = -1150.0f;


// * Player Variables //
// decrease the player's hurtbox size (hurtbox is normally the same size as the given texture)
int hurtboxLeewayWidth = 8;
int hurtboxLeewayHeight = 8;

// Player speed
float playerForceAmount = 750.0f;
// The intensity of the friction the player receives
float playerFrictionCoefficient = 1.0f;
float playerMaxVel = 1500.0f;
float playerMass = 100.0f;
int maxHealth = 3;
float maxIntangibleTime = 3.0f;


// * Car Variables //
// Static variables
float carMaxVel = 400.0f;
float carMass = 100.0f;
// The intensity of the friction the car receives
float carFrictionCoefficient = 1.0f;

// Min and Max of Horizontal and Vertical Speed of a car
float carForceAmountMin = 50.0f;
float carForceAmountMax = 400.0f;

// Min and Max of the horizontal multiplier (horizontalForce = forceAmount * horizontalMultiplier)
float horizontalMultiplierMin = 0.0f;
float horizontalMultiplierMax = 1.5f;

// Min and Max of the vertical spawn location of the cars
// (0.0f is the top of the game window, higher numbers will have the car spawn higher above the game window)
float verticalSpawnLocationMin = 0.0f;
float verticalSpawnLocationMax = windowSize.y * 0.5f;

// Maximum amount of cars that can exist
int carsStartMaxAmount = 3;
int carsMaxAmount = carsStartMaxAmount;
// The amount of cars that spawn at the start of the game
int carsStartAmount = 2;
// The current count of cars;
int carsAmount = 0;

// The max amount of time it takes for a car to spawn whenever carsAmount < carsMaxAmount
float carsMaxSpawnTime = 3.0f;
// The actual time it takes for a car to spawn
float carsDesiredSpawnTime = carsMaxSpawnTime;
// The timer for spawning cars
float carsSpawnTimer = 0.0f;


// * Road Markings //
float roadMarkingWidth = 5.0f;
float roadMarkingHeight = 20.0f;
float roadMarkingdistance = 40.0f;
int roadMarkingLineAmount = 5;


// * Float randomizer //
float randf(float min, float max) { return ((float)rand() / RAND_MAX) * (max - min) + min; }

// * loading textures //
sf::Texture loadTexture(const string fileName)
{
    sf::Texture texture = sf::Texture();
    if (!texture.loadFromFile("textures/" + fileName)) { cout << "Could not load image" << endl; }
    return texture;
}

// Loads a list of textures
list<sf::Texture> loadTexture(const list<string> fileNameList)
{
    list<sf::Texture> textureList;
    for (const string fileName : fileNameList) { textureList.push_back(loadTexture(fileName)); }
    return textureList;
}


// * Rigidbody initializers //
Player& playerInitializer(sf::Texture& playerTexture)
{
    Player* player = new Player{idCounter++, (int)playerTexture.getSize().x - hurtboxLeewayWidth, (int)playerTexture.getSize().y - hurtboxLeewayHeight,
        playerMaxVel, playerForceAmount, playerFrictionCoefficient, playerMass, maxHealth, maxIntangibleTime};

    player->loadSprite(playerTexture);
    player->setPosition(Vector2{windowSize.x * 0.5f, 0.0f});

    rbList.push_back(player);

    return *player;
}

void carInitializer(list<sf::Texture>& carTextureList, float cameraVerticalPos)
{
    auto it = carTextureList.begin();
    // Randomize car type / texture
    advance(it, rand() % carTextureList.size());

    // Get dimensions
    int width = it->getSize().x;
    int halfWidth = width / 2;
    int height = it->getSize().y;

    // Randomize variables
    float forceAmountPerFrame = randf(carForceAmountMin, carForceAmountMax);
    float horizontalMultiplier = randf(horizontalMultiplierMin, horizontalMultiplierMax);
    float verticalSpawnLocation = randf(verticalSpawnLocationMin, verticalSpawnLocationMax);

    // Initialize Car and push_back into rbList
    rbList.push_back(new Car{idCounter++, width, height, carMaxVel, forceAmountPerFrame,
        carFrictionCoefficient, carMass, horizontalMultiplier, bool(rand() % 2)});
    rbList.back()->loadSprite(*it);
    // Randomize spawn position
    rbList.back()->setPosition(Vector2{((float)rand() / RAND_MAX) * (windowSize.x - width) + halfWidth, -height * 0.5f - verticalSpawnLocation + cameraVerticalPos});

    carsAmount++;
}


// * main //
int main(int, char**){
    sf::RenderWindow window{sf::VideoMode(750, 1250), "Speed Racer"};
    window.setFramerateLimit(60);

    windowSize = {(float)window.getSize().x, (float)window.getSize().y};

    // Get seed for randomizer
    srand((unsigned int)time(nullptr));


    // * Initialize Player //
    sf::Texture& playerTexture = loadTexture("motorcycle.png");
    Player& player = playerInitializer(playerTexture);


    // * Initialize Cars //
    list<sf::Texture>& carTextureList = loadTexture
        ({"carBlack.png", "carBlue.png", "carGreen.png",
            "carOrange.png", "carYellow.png"});

    // Spawn Cars
    for (int i = 0; i < carsStartAmount; i++) { carInitializer(carTextureList, cameraVerticalOffset); }


    // * Background Elements //
    sf::Uint8 grayValue = (sf::Uint8)50.0f;
    sf::Color gray{grayValue, grayValue, grayValue};

    // White lines in the background
    sf::RectangleShape rectRoadMarking{sf::Vector2f(roadMarkingWidth, roadMarkingHeight)};
    // Set the origin to middle top of rectangle
    rectRoadMarking.setOrigin(rectRoadMarking.getSize().x * 0.5f , rectRoadMarking.getSize().y);


    // * UI panels //
    list<sf::Texture>& panelTextureList = loadTexture(list<string>{"panelBlue.png", "panelRed.png"});
    list<sf::Sprite> panelSpriteList;
    for (sf::Texture& texture : panelTextureList) { panelSpriteList.push_back(sf::Sprite{texture}); }

    // * UI hearts //
    list<sf::Texture>& heartTextureList = loadTexture(list<string>{"heartFull.png", "heartEmpty.png"});
    list<sf::Sprite> heartSpriteList;
    for (sf::Texture& texture : heartTextureList) { heartSpriteList.push_back(sf::Sprite{texture}); }

    // * UI Text //
    sf::Font font;
    if (!font.loadFromFile("fonts/Super Cartoon.ttf"))
    {
        cout << "Could not find font file" << endl;
    }
    sf::Text text{"", font};
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(5.0f);


    // Set up clock for deltaTime
    sf::Clock clock;

    float score = 0.0f;
    int carsDodged = 0;

    bool holdLeft = false;
    bool holdRight = false;
    bool holdUp = false;
    bool holdDown = false;

    bool gameOver = false;

    while(window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            // * Player Inputs //
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();

                if (event.key.code == sf::Keyboard::A) holdLeft = true;
                if (event.key.code == sf::Keyboard::D) holdRight = true;
                if (event.key.code == sf::Keyboard::W) holdUp = true;
                if (event.key.code == sf::Keyboard::S) holdDown = true;
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::A) holdLeft = false;
                if (event.key.code == sf::Keyboard::D) holdRight = false;
                if (event.key.code == sf::Keyboard::W) holdUp = false;
                if (event.key.code == sf::Keyboard::S) holdDown = false;
            }
        }

        if (!gameOver)
        {
            // Get deltaTime and restart clock
            float deltaTime = clock.restart().asSeconds();

            // Increase difficulty by the amount traveled, this increases the maximum amount of cars
            carsMaxAmount = carsStartMaxAmount + (int)(-player.pos->y / diffIncrDistance);

            // Score counter (carsDodged + amountTraveled)
            score = carsDodged * scoreForDodging + -player.pos->y * scoreForTravel;

            // Get cameraPosition
            cameraPosition.y = player.pos->y + cameraVerticalOffset;


            // * Draw Background //
            window.clear(gray);
            
            // Draw white stripes
            for (int i = 0; i < roadMarkingLineAmount + 2 ; i++)
            {
                float distanceBetweenOrigin = rectRoadMarking.getSize().y + roadMarkingdistance;
                for (int j = 0; j < windowSize.y / rectRoadMarking.getSize().y * 0.5f; j++)
                {
                    rectRoadMarking.setPosition(windowSize.x / (roadMarkingLineAmount + 1) * i, 
                        j * distanceBetweenOrigin - ((int)cameraPosition.y % (int)distanceBetweenOrigin));
                    window.draw(rectRoadMarking);
                }
            }


            // * Spawn cars //
            if (carsSpawnTimer >= carsDesiredSpawnTime)
            {
                if (carsAmount < carsMaxAmount)
                {
                    carInitializer(carTextureList, cameraPosition.y);

                    // Reset timer
                    carsSpawnTimer = 0.0f;
                    carsDesiredSpawnTime = carsMaxSpawnTime / MyMathLib::max(carsMaxAmount - carsAmount, 1);
                }
            }
            else { carsSpawnTimer += deltaTime; }


            // * Update rigidBody objects //
            for (auto it = rbList.begin(); it != rbList.end(); it++)
            {
                RigidBody& rbObject = **it;

                if (rbObject.faction == Faction::PLAYER) { continue; }

                // Check if rbObject is dead
                if (!rbObject.update(rbList, windowSize, window, cameraPosition, deltaTime))
                {
                    score += 10.0f;
                    it = rbList.erase(it);
                    delete &rbObject;
                    carsAmount--;
                    carsDodged++;

                    it--;
                }
            }


            // * Update Player // 
            player.movementLogic(holdLeft, holdRight, holdUp, holdDown, deltaTime);

            // If player gets hit
            if (player.update(rbList, windowSize, window, cameraPosition, deltaTime))
            {
                player.hit = false; // Reset the hit boolean
                gameOver = player.health <= 0;
            }


            // * Draw UI //
            for (int i = 1; i <= player.maxHealth; i++)
            {
                sf::Sprite sprite;
                if (i <= player.health) { sprite = heartSpriteList.front(); }  // Get full heart sprite
                else { sprite = heartSpriteList.back(); }   // Get empty heart sprite

                sprite.setPosition(windowSize.x - i * ((float)sprite.getTexture()->getSize().x + 10.0f) - 15.0f, 20.0f);
                window.draw(sprite);
            }

            text.setString("Score: " + to_string(MyMathLib::round(score, 2)));
            text.setPosition(20.0f, 20.0f);
            window.draw(text);


            // When player is dead
            if (gameOver)
            {
                cout << "Game Over" << endl;

                sf::Sprite sprite;
                string additionalText;
                
                // Show win or lose screen depending on score
                if (score >= winCondition)
                {
                    text.setString("You Win!");
                    additionalText = "Your score reached past 1000";
                    sprite = panelSpriteList.front();
                }
                else
                {
                    text.setString("You Lose!");
                    additionalText = "Your score didn't reach past 1000";
                    sprite = panelSpriteList.back();
                }

                // Panel
                sprite.setOrigin((float)sprite.getTexture()->getSize().x / 2.0f, (float)sprite.getTexture()->getSize().y / 2.0f);
                sprite.setPosition(windowSize.x / 2, windowSize.y / 2);
                window.draw(sprite);

                // Text
                text.setCharacterSize(72);
                sf::FloatRect bounds = text.getLocalBounds();
                text.setPosition((windowSize.x - bounds.width) / 2, windowSize.y / 2 - bounds.height * 1.5f);
                window.draw(text);

                text.setString(additionalText);
                text.setCharacterSize(24);
                bounds = text.getLocalBounds();
                text.setPosition((windowSize.x - bounds.width) / 2, windowSize.y / 2 + bounds.height * 1.5f);
                window.draw(text);
            }

            window.display();
        }
    }

    // * Delete RigidBody Objects //
    for (int i = (int)rbList.size(); i > 0; i--)
    {
        auto it = rbList.begin();
        delete *it;
        rbList.erase(it);
    }

    return 0;
}