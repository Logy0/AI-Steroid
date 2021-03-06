#include "../headers/HasPhysics.hpp"
#include <iostream>

HasPhysics::HasPhysics( uint8_t id , const sf::Vector2f& pos , const sf::Vector2f& vel , const sf::Vector2f& acc,
                        float rot , float vel_rot , float acc_rot) :
    m_typeId(id),
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
        lifeTime+=timeElapsed;
        if( maxLifeTime != 0 && lifeTime > maxLifeTime )
            bWantKill = true;

        if(m_frictions_enabled[0])
            m_vel.x+=(m_acc.x-m_fric*m_vel.x)*timeElapsed;
        if(m_frictions_enabled[1])
            m_vel.y+=(m_acc.y-m_fric*m_vel.y)*timeElapsed;
        if(!m_frictions_enabled[0] && !m_frictions_enabled[1])
            m_vel+=m_acc*timeElapsed;

        if(m_max_vel != 0)
        {
            if(m_vel.x > m_max_vel)m_vel.x=m_max_vel;
            else if(m_vel.x < -m_max_vel)m_vel.x=-m_max_vel;
            if(m_vel.y > m_max_vel)m_vel.y=m_max_vel;
            else if(m_vel.y < -m_max_vel)m_vel.y=-m_max_vel;
        }

        m_pos+=m_vel*timeElapsed;

        if(m_frictions_enabled[2])
            m_vel_rot+=(m_acc_rot-m_fric_rot*m_vel_rot)*timeElapsed;
        else
            m_vel_rot+=m_acc_rot*timeElapsed;

        m_rot+=m_vel_rot*timeElapsed;
}
