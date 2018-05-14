#include "../headers/Asteroid.hpp"

Asteroid::Asteroid( const sf::Texture& texture , const float mean_radius , const std::array<sf::Vector2f,NB_OF_VERTEX_ASTEROIDS>& vertices , const sf::Vector2f& pos , const sf::Vector2f& vel , const sf::Vector2f& acc , float rot , float vel_rot , float acc_rot ) : HasPhysics(1,pos,vel,acc,rot,vel_rot,acc_rot)
{
    shape.setPointCount(NB_OF_VERTEX_ASTEROIDS);
    shape.setPosition(pos);
    shape.setTexture(&texture);
    collide_box.setRadius(mean_radius);
    collide_box.setPosition(pos-sf::Vector2f(mean_radius,mean_radius));
    collide_box.setOutlineThickness(1);
    collide_box.setOutlineColor(sf::Color::Red);
    collide_box.setFillColor(sf::Color::Transparent);
    for( size_t i=0 ; i<NB_OF_VERTEX_ASTEROIDS; i++)
    {
        shape.setPoint(i,vertices[i]);
    }
}

void Asteroid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    shape.setPosition(m_pos);
    shape.setRotation(m_rot);
    collide_box.setPosition(m_pos-sf::Vector2f(collide_box.getRadius(),collide_box.getRadius()));
    target.draw(shape,states);
    target.draw(collide_box,states);
}

void Asteroid::update(float timeElapsed)
{
    updatePhysics(timeElapsed);
}

void Asteroid::applyDamages(uint16_t damages)
{
    if(damages >= lifePoints)bWantKill=true;
    else lifePoints-=damages;
}
