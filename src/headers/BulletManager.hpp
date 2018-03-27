#ifndef BULLETMANAGER_HPP
#define BULLETMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <set>
#include "TextureManager.hpp"
#include "Bullet.hpp"

class BulletManager : public sf::Drawable
{
private:
    TextureManager& textureManager;
    std::set<Bullet*> list;
public:
    BulletManager(TextureManager&);
    void generate( const sf::Vector2f& pos );
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(float timeElapsed);
    void clear();
    inline const std::set<Bullet*>& getBullets() const { return list; }
};

#endif //
