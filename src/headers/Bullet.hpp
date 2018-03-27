#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>
#include "HasPhysics.hpp"

class Bullet : public sf::Drawable , public HasPhysics
{
    private:
        mutable sf::RectangleShape shape;
    public:
        Bullet(const sf::Texture& , const sf::Vector2f& pos, const sf::Vector2f& vel);
        virtual void draw(sf::RenderTarget&,sf::RenderStates) const;
        void update(float timeElapsed);
        virtual void onMaxLifeTimeExceeded();
};

#endif //BULLET_HPP
