#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <time.h>
#include <windows.h>

//dictionnaire Activation Key
std::unordered_map<sf::Keyboard::Key, bool> KeyPressed = {
    {sf::Keyboard::Z, false},
    {sf::Keyboard::S, false},
    {sf::Keyboard::Q, false},
    {sf::Keyboard::D, false}
};

bool RCCollision(sf::RectangleShape Rectangle, sf::CircleShape Circle);


int main()
{
    //settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;    
    //définis la window et sa taille
    sf::RenderWindow window(sf::VideoMode(700, 700), "Game", sf::Style::Close, settings);
    //définis la texture
    sf::Texture PlayerTexture;
    //load la texture
    PlayerTexture.loadFromFile("C:/SFMLProject/PlayerSprite.png");
    //vérifie si la texture se load
    if (!PlayerTexture.loadFromFile("C:/SFMLProject/PlayerSprite.png"))
    {
        std::cout << "failed to load texture";
    }
    //rend la texture plus belle
    PlayerTexture.setSmooth(true);
    //définis le Sprite du joueur
    sf::Sprite PlayerSprite;
    //Integre la texture au sprite
    PlayerSprite.setTexture(PlayerTexture);
    //Set la localisation du Sprite
    PlayerSprite.setOrigin(sf::Vector2f(68.f, 50.f));
    PlayerSprite.setPosition(350, 350);
    float PlayerWeight = 32;
    float structureWeight = 50000;
    float MeteorWeight = 100;
    //frame rate
    sf::Clock Clock;
    //Current Position
    sf::Vector2f CurrentPosition;
    sf::Vector2f CurrentMeteorPosition;
    //définition du Sprite météor
    sf::Texture MeteorTexture;
    sf::Sprite MeteorSprite;
    MeteorTexture.loadFromFile("C:/SFMLProject/asteroid.png");
    if (!MeteorTexture.loadFromFile("C:/SFMLProject/asteroid.png"))
    {
        std::cout << "failed to load texture";
    }
    MeteorSprite.setTexture(MeteorTexture);
    MeteorSprite.setOrigin(sf::Vector2f(77.f, 54.f));
    MeteorSprite.setPosition(100, 30);
    MeteorSprite.setScale(0.4f, 0.4f);
    MeteorTexture.setSmooth(true);
    // gravity implementation
    float GravityFormula;
    const sf::Vector2f Gravity(0.f , 50.f);
    // random int
    int randX;
    // frixion
    float FrictionCoef;
    int Force;
    sf::Vector2f Velocity(0.f, 0.f);
    sf::Vector2f MaxVelocity(1000.f, 10.f);
    //hitbox player
    sf::RectangleShape PlayerHitbox(sf::Vector2f(136, 100));
    PlayerHitbox.setFillColor(sf::Color(0, 0, 0, 0));
    PlayerHitbox.setOutlineThickness(1);
    PlayerHitbox.setOutlineColor(sf::Color(250, 150, 100));
    PlayerHitbox.setOrigin(sf::Vector2f(68.f, 50.f));
    //hitbox meteor
    sf::CircleShape MeteorHitbox(35);
    MeteorHitbox.setFillColor(sf::Color(0, 0, 0, 0));
    MeteorHitbox.setOutlineThickness(1);
    MeteorHitbox.setOutlineColor(sf::Color(250, 150, 100));
    MeteorHitbox.setOrigin(sf::Vector2f(38.f,   38.f));
    //collisions variables
    float MeteorRadius;
    sf::Vector2f SizePlayerHitbox;


    //Game loop
    while (window.isOpen())
    {
        //Event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ) {
                window.close();
            }
            
            //Event si une touche du clavier est pressé
            if (event.type == sf::Event::KeyPressed) {
                auto it = KeyPressed.find(event.key.code);
                if (it != KeyPressed.end()) {
                    it->second = true;
                }
                
            }
            //Event si une touche du clavier est lâchée
            if (event.type == sf::Event::KeyReleased){
                auto it = KeyPressed.find(event.key.code);
                if (it != KeyPressed.end()) {
                    it->second = false;
                }
            }


        }

        float dt = Clock.restart().asSeconds();
        
        
        //checks if Z is pressed
        auto Z = KeyPressed.find(sf::Keyboard::Z);
        if (Z != KeyPressed.end()) {
            if (Z->second) {
                Velocity.y -= 0.25 * dt;
            }            
        }
        //checks if Q is pressed
        auto Q = KeyPressed.find(sf::Keyboard::Q);
        if (Q != KeyPressed.end()) {
            if (Q->second) {
                Velocity.x -= 0.25 * dt;
            }
        }
        //checks if S is pressed
        auto S = KeyPressed.find(sf::Keyboard::S);
        if (S != KeyPressed.end()) {
            if (S->second) {
                Velocity.y += 0.25 * dt;
            }
        }
        //checks if D is pressed
        auto D = KeyPressed.find(sf::Keyboard::D);
        if (D != KeyPressed.end()) {
            if (D->second) {
                Velocity.x += 0.25 * dt;
            }
        }
        
        //PlayerSprite.move(Velocity.x, Velocity.y);
        
        //Set la position du Player à l'opposé de l'écran si il traverse les limites de l'écran
        CurrentPosition = PlayerSprite.getPosition();
        if (CurrentPosition.y < 0) {
            PlayerSprite.setPosition(CurrentPosition.x, 700);
        }
        if (CurrentPosition.y > 700) {
            PlayerSprite.setPosition(CurrentPosition.x, 0);
        }
        if (CurrentPosition.x > 700) {
            PlayerSprite.setPosition(0, CurrentPosition.y);
        }
        if (CurrentPosition.x < 0) {
            PlayerSprite.setPosition(700, CurrentPosition.y);
        }

        //physics application
        FrictionCoef = 1.5;
        Velocity.x = Velocity.x * (1 - FrictionCoef * dt);
        Velocity.y = Velocity.y * (1 - FrictionCoef * dt);
        PlayerSprite.move(Velocity);
        

        //meteor movment
        MeteorSprite.rotate(500 * dt);
        MeteorSprite.move(0, 5 * dt);
        CurrentMeteorPosition = MeteorSprite.getPosition();

        //hitboxes positions
        PlayerHitbox.setPosition(CurrentPosition);
        MeteorHitbox.setPosition(CurrentMeteorPosition);

        //collision detection
        MeteorRadius = MeteorHitbox.getRadius();
        SizePlayerHitbox = PlayerHitbox.getSize();
        if (RCCollision(PlayerHitbox, MeteorHitbox))
        {
            std::cout << "collision !" << "\n";
        }


        //game drawing/rendering
        window.clear();
        window.draw(PlayerSprite);
        window.draw(MeteorSprite);
        window.draw(PlayerHitbox);
        window.draw(MeteorHitbox);
        window.display();

    }

    return 0;
}


bool RCCollision(sf::RectangleShape Rectangle, sf::CircleShape Circle)
{
    if (abs(Rectangle.getPosition().x - Circle.getPosition().x) < abs(Circle.getRadius() + Rectangle.getSize().x / 2) && abs(Rectangle.getPosition().y - Circle.getPosition().y) < abs(Circle.getRadius() + Rectangle.getSize().y / 2)) {
        return true;
    }
    else
    {
        return false;
    }
}