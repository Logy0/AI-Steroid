//
// Created by asphox on 21/05/18.
//

#include "../headers/PowerUp.hpp"

PowerUp::PowerUp(TextureManager& textureManager, const sf::Vector2f &pos, uint8_t id)
        :HasPhysics(id,pos,{0,0},{0,0},0,0,0)
        ,textureManager(textureManager)
{
    shape.setSize(sf::Vector2f(30,30));
    shape.setOrigin(sf::Vector2f(15,15));
    shape.setPosition(pos);
}

void PowerUp::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    shape.setPosition(m_pos);
    shape.setRotation(m_rot);
    target.draw(shape,states);
}

void PowerUp::update(float timeElapsed)
{
    updatePhysics(timeElapsed);
}

PowerUp::~PowerUp()
{
}