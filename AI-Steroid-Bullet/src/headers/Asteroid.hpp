#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <array>
#include <SFML/Graphics.hpp>
#include "Defines.hpp"
#include "HasPhysics.hpp"
#include "Trajectory.hpp"

class AsteroidManager;

class Asteroid : public sf::Drawable , public HasPhysics
{
    private:
        mutable sf::ConvexShape shape;
        mutable sf::CircleShape collide_box;
        uint16_t lifePoints = 1;
    public:
        Asteroid( const sf::Texture& , const float mean_radius , const std::array<sf::Vector2f,NB_OF_VERTEX_ASTEROIDS>& vertices , const sf::Vector2f& pos , const sf::Vector2f& vel , const sf::Vector2f& acc , float rot , float vel_rot , float acc_rot );
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update(float timeElapsed);
        inline float getMeanRadius(){ return collide_box.getRadius(); }
        void applyDamages(uint16_t damages);
};

#endif //ASTEROID_HPP
