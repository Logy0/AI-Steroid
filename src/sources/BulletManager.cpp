#include "../headers/BulletManager.hpp"

BulletManager::BulletManager(TextureManager& textureManager)
:textureManager(textureManager)
{

}

void BulletManager::generate(const sf::Vector2f& pos)
{
    Bullet* tmp = new Bullet(textureManager.get("bullet"),pos,{0,-1000});
    tmp->setMaxLifeTime(0.5);
    list.insert( tmp );
}

void BulletManager::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
    for( auto& it : list )
    {
        target.draw(*it,states);
    }
}

void BulletManager::update(float timeElapsed)
{
    for( auto it = list.begin() ; it != list.end() ; )
    {
        if((*it)->wantedKill())
        {
            delete *it;
            it = list.erase(it);
        }
        else
        {
            (*it)->update(timeElapsed);
            ++it;
        }
    }
}
