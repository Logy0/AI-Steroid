#ifndef HAS_PHYSICS_HPP
#define HAS_PHYSICS_HPP

#include <SFML/Graphics.hpp>

class HasPhysics
{
    protected:
    sf::Vector2f m_pos;
    sf::Vector2f m_vel;
    sf::Vector2f m_acc;

    float m_rot;
    float m_vel_rot;
    float m_acc_rot;

    uint8_t m_typeId = 0;

    public:
    HasPhysics( const sf::Vector2f& pos , const sf::Vector2f& vel , const sf::Vector2f& acc,
                float rot , float vel_rot , float acc_rot );

    //void setPosition( const sf::Vector2f& pos);
    //void setVelocity( const sf::Vector2f& vel );
    //void setAcceleration( const sf::Vector2f& acc );

    inline const sf::Vector2f& getPosition() const { return m_pos; }
    inline uint8_t getTypeId() const { return m_typeId; }
    //inline const sf::Vector2f& getVelocity() const { return vel; }
    //inline const sf::Vector2f& getAcceleration() const { return acc; }
    virtual void updatePhysics( float timeElapsed );
};

#endif //HAS_PHYSICS_HPP
