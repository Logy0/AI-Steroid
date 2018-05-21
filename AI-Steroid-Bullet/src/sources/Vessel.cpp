#include "../headers/Vessel.hpp"

Vessel::Vessel( TextureManager& textureManager, BulletManager& bulletManager , const sf::Vector2f& pos )
    :HasPhysics(2,pos,{0,0},{0,0},0,0,0)
    ,bulletManager(bulletManager)
    ,textureManager(textureManager)
{
    shape.setSize(sf::Vector2f(36,22));
    shape.setOrigin(sf::Vector2f(18,11));
    shape.setPosition(pos);
    setMaxVelocity(400);
    setFriction(FRICTION_VESSEL);
    animation = new Animation(textureManager.get("vessel"),shape,true);
}

void Vessel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    shape.setPosition(m_pos);
    shape.setRotation(m_rot);
    target.draw(shape,states);
}

void Vessel::update(float timeElapsed)
{
    updatePhysics(timeElapsed);
}

void Vessel::thrust(THRUST_DIRECTION tdir)
{
    float acc_x=0,acc_y=0;
    if(tdir == THRUST_DIRECTION::NONE)
    {
        enableFrictions(true);
    }
    else
    {
        enableFrictions(false);
        if(tdir & THRUST_DIRECTION::UP)
        {
            acc_y-=MAX_THRUST_VESSEL;
        }
        if(tdir & THRUST_DIRECTION::LEFT)
        {
            acc_x-=MAX_THRUST_VESSEL;
        }
        if(tdir & THRUST_DIRECTION::DOWN)
        {
            acc_y+=MAX_THRUST_VESSEL;
        }
        if(tdir & THRUST_DIRECTION::RIGHT)
        {
            acc_x+=MAX_THRUST_VESSEL;
        }
    }
    m_acc={acc_x,acc_y};
}
void Vessel::settexture(TextureManager& manager)
{
	shape.setTexture(&manager.get("vessel"));
}
void Vessel::reset(sf::Vector2f respos)
{
	m_pos = respos;
}
void Vessel::fire()
{
    if(!fireLocked)
    {
        bulletManager.generate(m_pos);
        fireLocked = true;
    }
}

void Vessel::releaseFire()
{
    fireLocked = false;
}

Vessel::~Vessel()
{
    delete animation;
}
