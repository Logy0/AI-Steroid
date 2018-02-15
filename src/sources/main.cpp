#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <iostream>
#include "../headers/Defines.hpp"
#include "../headers/AsteroidManager.hpp"
#include "../headers/Asteroid.hpp"
#include "../headers/TextureManager.hpp"
#include "../headers/IAGridVisual.hpp"


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

    manager.generate({70,60});
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        manager.update(sec);
        clock.restart();
        window.clear();
        window.draw(manager);
        if(ENABLE_GRID_TEST_MOD)
        {
            grid.reset();
            for( const auto& it : manager.getAsteroids())
            {
                grid.update(it);
            }
            grid.updateVisual();
            window.draw(grid);
        }
        window.display();
        sec = clock.getElapsedTime().asSeconds();
    }
}
