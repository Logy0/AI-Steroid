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
#include "../headers/BulletManager.hpp"
#include "../headers/CollisionsManager.hpp"



int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_X,WINDOW_Y),"Test genetaration");
    window.setFramerateLimit(60);

    sf::Clock clock;
    float sec = 0;

    IAGridVisual grid;
    TextureManager textureMgr;
    textureMgr.add("asteroid",PATH_TEXTURES+"asteroids.png");
    textureMgr.add("bullet",PATH_TEXTURES+"bullet.png");
    textureMgr.add("vessel",PATH_TEXTURES+"vessel.png");

    AsteroidManager asteroidManager(textureMgr);
    BulletManager bulletManager(textureMgr);
    Vessel vessel(textureMgr,bulletManager,{200,200});
    CollisionsManager collisionsManager(vessel,asteroidManager,bulletManager);

    uint8_t tdir = THRUST_DIRECTION::NONE ;

    bool display_grid = false;

	Asteroid* ast = asteroidManager.generate({ 70,60 }, {10,0});
	Cos_Trajectory traj(0,0,0,100,2,0);
	ast->setTrajectory(traj);
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
                if(event.key.code == sf::Keyboard::Space)
                    vessel.releaseFire();
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
                if(event.key.code == sf::Keyboard::Space)
                    vessel.fire();
            }
        }
        asteroidManager.update(sec);
        bulletManager.update(sec);
        vessel.thrust(static_cast<THRUST_DIRECTION>(tdir));
        vessel.update(sec);
        clock.restart();
        window.clear();
        window.draw(asteroidManager);
        window.draw(vessel);
        window.draw(bulletManager);
        collisionsManager.check();
        if(ENABLE_GRID_TEST_MOD)
        {
            grid.reset();
            grid.update(&vessel);
            for( const auto& it : asteroidManager.getAsteroids())
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
	return 0;
}
