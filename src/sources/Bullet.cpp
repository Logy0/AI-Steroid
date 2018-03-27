#include "../headers/Bullet.hpp"

Bullet::Bullet( const sf::Texture& texture, const sf::Vector2f& pos, const sf::Vector2f& vel)
:HasPhysics(0,pos,vel,{0,0},0,0,0)
{
    shape.setTexture(&texture);
    shape.setSize({5,10});
}
void Bullet::draw(sf::RenderTarget& target,sf::RenderStates state) const
{
    shape.setPosition(m_pos);
    target.draw(shape,state);
}
void Bullet::update(float timeElapsed)
{
    updatePhysics(timeElapsed);
}

void Bullet::onMaxLifeTimeExceeded()
{

}
