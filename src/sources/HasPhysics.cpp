#include "../headers/HasPhysics.hpp"
#include "../headers/Defines.hpp"
#include <iostream>

HasPhysics::HasPhysics( const sf::Vector2f& pos , const sf::Vector2f& vel , const sf::Vector2f& acc,
                        float rot , float vel_rot , float acc_rot ) :
    m_pos(pos),
    m_vel(vel),
    m_acc(acc),
    m_rot(rot),
    m_vel_rot(vel_rot),
    m_acc_rot(acc_rot)
{
}

void HasPhysics::updatePhysics(float timeElapsed)
{
    m_vel+=m_acc*timeElapsed;
    m_pos+=m_vel*timeElapsed;
	if (m_pos.x >= WINDOW_X)m_pos.x = 0;
	if (m_pos.y >= WINDOW_Y)m_pos.y = 0;
	if (m_pos.x < 0)m_pos.x = WINDOW_X;
	if (m_pos.y < 0)m_pos.y = WINDOW_Y;
    m_vel_rot+=m_acc_rot*timeElapsed;
    m_rot+=m_vel_rot*timeElapsed;
}
