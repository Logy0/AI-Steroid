//
// Created by asphox on 21/05/18.
//

#ifndef AI_STEROID_POWERUP_HPP
#define AI_STEROID_POWERUP_HPP

#include <SFML/Graphics.hpp>
#include "HasPhysics.hpp"
#include "Animation.hpp"
#include "TextureManager.hpp"

class PowerUp : public sf::Drawable , public HasPhysics
{
protected:
    TextureManager& textureManager;
    Animation* animation;
    mutable sf::RectangleShape shape;

public:
    PowerUp( TextureManager&, const sf::Vector2f& pos, uint8_t id);
    virtual ~PowerUp() = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(float timeElapsed);
};

class Life_PowerUp : public PowerUp
{
public:
    Life_PowerUp(TextureManager& _textureManager,const sf::Vector2f& pos) : PowerUp(_textureManager,pos,3)
    {
        animation = new Animation(textureManager.get("life"),shape,true);
    }

    ~Life_PowerUp()
    {
        delete animation;
    }
};

class Speed_PowerUp : public PowerUp
{
public:
    Speed_PowerUp(TextureManager& _textureManager,const sf::Vector2f& pos) : PowerUp(_textureManager,pos,4)
    {
        animation = new Animation(textureManager.get("speed"),shape,true);
    }

    ~Speed_PowerUp()
    {
        delete animation;
    }
};

#endif //AI_STEROID_POWERUP_HPP
