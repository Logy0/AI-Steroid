#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <iostream>
#include "../headers/Defines.hpp"
#include "../headers/AsteroidManager.hpp"
#include "../headers/Asteroid.hpp"
#include "../headers/TextureManager.hpp"
#include "../headers/IAGridVisual.hpp"
#include "../headers/Vessel.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_X,WINDOW_Y),"Test genetaration");
    window.setFramerateLimit(60);

    sf::Clock clock;
    float sec = 0;

    IAGridVisual grid;
    TextureManager textureMgr;
    textureMgr.add("asteroid",PATH_TEXTURES+"asteroids.png");
    AsteroidManager manager(textureMgr);
    Vessel vessel({200,200});
    uint8_t tdir = THRUST_DIRECTION::NONE ;

    bool display_grid = false;

    manager.generate({70,60});
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::Z)
                    tdir&=~THRUST_DIRECTION::UP;
                if(event.key.code == sf::Keyboard::S)
                    tdir&=~THRUST_DIRECTION::DOWN;
                if(event.key.code == sf::Keyboard::Q)
                    tdir&=~THRUST_DIRECTION::LEFT;
                if(event.key.code == sf::Keyboard::D)
                    tdir&=~THRUST_DIRECTION::RIGHT;
            }
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::G)
                    display_grid=!display_grid;
                if(event.key.code == sf::Keyboard::Z)
                    tdir|=THRUST_DIRECTION::UP;
                if(event.key.code == sf::Keyboard::S)
                    tdir|=THRUST_DIRECTION::DOWN;
                if(event.key.code == sf::Keyboard::Q)
                    tdir|=THRUST_DIRECTION::LEFT;
                if(event.key.code == sf::Keyboard::D)
                    tdir|=THRUST_DIRECTION::RIGHT;
            }
        }
        manager.update(sec);
        vessel.thrust(static_cast<THRUST_DIRECTION>(tdir));
        vessel.update(sec);
        clock.restart();
        window.clear();
        window.draw(manager);
        window.draw(vessel);
        if(ENABLE_GRID_TEST_MOD)
        {
            grid.reset();
            grid.update(&vessel);
            for( const auto& it : manager.getAsteroids())
            {
                grid.update(it);
            }
            if(display_grid)
            {
                grid.updateVisual();
                window.draw(grid);
            }
        }
        window.display();
        sec = clock.getElapsedTime().asSeconds();
    }
}
