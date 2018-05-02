#ifndef ASTEROID_MANAGER_HPP
#define ASTEROID_MANAGER_HPP

#include <set>
#include <vector>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Defines.hpp"
#include "Asteroid.hpp"
#include "TextureManager.hpp"

class AsteroidManager : public sf::Drawable
{
    private:
        const TextureManager& texturesMgr;
        std::set<Asteroid*> list;
        std::default_random_engine rand;
        std::normal_distribution<float> dist_variations_radius;
        std::normal_distribution<float> dist_base_radius;
    public:
        AsteroidManager(const TextureManager&);
        ~AsteroidManager();
        Asteroid* generate( const sf::Vector2f& position, const sf::Vector2f& velociraptor);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update(float timeElapsed);
        void clear();
        inline const std::set<Asteroid*>& getAsteroids() const{ return list; }


};

#endif //ASTEROID_MANAGER_HPP
