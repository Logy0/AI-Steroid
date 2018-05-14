#include "../headers/CollisionsManager.hpp"

CollisionsManager::CollisionsManager(Vessel& vessel,AsteroidManager& asteroidManager,BulletManager& bulletManager)
    :vessel(vessel)
    ,asteroidManager(asteroidManager)
    ,bulletManager(bulletManager)
{
}

void CollisionsManager::check()
{
    for( auto itAsteroid = asteroidManager.getAsteroids().begin() ; itAsteroid != asteroidManager.getAsteroids().end() ; )
    {
        //basic collision vessel, à ameliorer
        if(Math::distance(vessel.getPosition(),(*itAsteroid)->getPosition()) <= (*itAsteroid)->getMeanRadius())
        {
			vessel.killed = true;
        }

        for( auto itBullet = bulletManager.getBullets().begin() ; itBullet != bulletManager.getBullets().end() ; )
        {
            if( Math::distance((*itBullet)->getPosition(),(*itAsteroid)->getPosition()) <= (*itAsteroid)->getMeanRadius() )
            {
                (*itAsteroid)->applyDamages(1);
                bulletManager.getBullets().erase(itBullet);
                break;
            }
            else
            {
                ++itBullet;
            }
        }
        ++itAsteroid;
    }
}
