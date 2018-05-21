#ifndef HAS_PHYSICS_HPP
#define HAS_PHYSICS_HPP

#include <SFML/Graphics.hpp>
#include <bitset>
#include <cstdlib>
#include "Trajectory.hpp"

class HasPhysics
{
    private:
        uint8_t m_typeId = 0;
        std::bitset<3> m_frictions_enabled=0; //x;y;r
    protected:

    sf::Vector2f m_pos;
    sf::Vector2f m_vel;
    float        m_max_vel = 0;
    sf::Vector2f m_acc;
    float        m_fric = 0;

    float       m_rot;
    float       m_vel_rot;
    float       m_acc_rot;
    float       m_fric_rot = 0;

    float lifeTime = 0;
    float maxLifeTime = 0;
    bool  bWantKill = false;

    Trajectory trajectory;

    public:
    HasPhysics( uint8_t id , const sf::Vector2f& pos , const sf::Vector2f& vel , const sf::Vector2f& acc,
                float rot , float vel_rot , float acc_rot );
    //void setPosition( const sf::Vector2f& pos);
    inline void setVelocity( const sf::Vector2f& vel ){ m_vel = vel;}
    //void setAcceleration( const sf::Vector2f& acc );

    inline const sf::Vector2f& getPosition() const { return m_pos; }
    inline uint8_t getTypeId() const { return m_typeId; }
    inline void enableFrictions(bool b){ if(b)m_frictions_enabled = 7;else m_frictions_enabled = 0; }
    inline void enableFrictionX(bool b){ m_frictions_enabled[0] = b;}
    inline void enableFrictionY(bool b){ m_frictions_enabled[1] = b;}
    inline void enableFrictionR(bool b){ m_frictions_enabled[2] = b;}
    inline void setMaxVelocity(float vel){ m_max_vel=vel; }
    inline void setFriction(float fric){ m_fric=fric;}
    inline void setFrictionRotation(float fric){ m_fric_rot=fric;}
    //inline const sf::Vector2f& getVelocity() const { return vel; }
    //inline const sf::Vector2f& getAcceleration() const { return acc; }
    virtual void updatePhysics( float timeElapsed );
    inline void setMaxLifeTime(float maxLifeTime){ this->maxLifeTime = maxLifeTime; }
    inline bool wantKill(){ return bWantKill; }
    inline void setTrajectory(const Trajectory& traj){ trajectory=traj; }

};

#endif //HAS_PHYSICS_HPP
