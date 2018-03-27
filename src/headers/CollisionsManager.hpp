#ifndef COLLISIONSMANAGER_HPP
#define COLLISIONSMANAGER_HPP

#include "AsteroidManager.hpp"
#include "BulletManager.hpp"
#include "Vessel.hpp"
#include "Math.hpp"
#include <iostream>

class CollisionsManager
{
private:
    Vessel&          vessel;
    AsteroidManager& asteroidManager;
    BulletManager&   bulletManager;
public:
    CollisionsManager(Vessel&,AsteroidManager&,BulletManager&);
    void check();
};

#endif //COLLISIONSMANAGER_HPP
