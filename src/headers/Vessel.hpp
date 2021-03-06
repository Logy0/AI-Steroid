#ifndef VESSEL_HPP
#define VESSEL_HPP

#include <SFML/Graphics.hpp>
#include "Defines.hpp"
#include "HasPhysics.hpp"
#include "BulletManager.hpp"
#include "TextureManager.hpp"

class Vessel : public sf::Drawable , public HasPhysics
{
    private:
        BulletManager& bulletManager;
        TextureManager& textureManager;
        mutable sf::RectangleShape shape;
    public:
        Vessel( TextureManager&, BulletManager& , const sf::Vector2f& pos );
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update(float timeElapsed);

        void thrust(THRUST_DIRECTION tdir);
        void fire();
};

#endif //VESSEL_HPP
