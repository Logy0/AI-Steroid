#ifndef VESSEL_HPP
#define VESSEL_HPP

#include <SFML/Graphics.hpp>
#include "Defines.hpp"
#include "HasPhysics.hpp"
#include "BulletManager.hpp"
#include "TextureManager.hpp"
#include "Animation.hpp"

class Vessel : public sf::Drawable , public HasPhysics
{
    private:
        BulletManager& bulletManager;
        TextureManager& textureManager;
        Animation* animation;

		mutable sf::RectangleShape shape;
		bool fireLocked = false;
    public:
		void reset(sf::Vector2f respos);
		void settexture(TextureManager& manager);
        Vessel( TextureManager&, BulletManager& , const sf::Vector2f& pos );
        ~Vessel();
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update(float timeElapsed);
		bool killed = false;
        void thrust(THRUST_DIRECTION tdir);
        void fire();
        void releaseFire();
};

#endif //VESSEL_HPP
