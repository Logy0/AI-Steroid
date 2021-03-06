#include "../headers/TextureManager.hpp"

void TextureManager::add( const std::string& name , const std::string& path )
{
    sf::Texture tmp;
    tmp.loadFromFile(path);
    m_map.insert({name,tmp});

}

void TextureManager::add( const std::string& name , const std::string& path , const sf::IntRect& area )
{
    sf::Texture tmp;
    tmp.loadFromFile(path,area);
    m_map.insert({name,tmp});
}

const sf::Texture& TextureManager::get( const std::string& name ) const
{
    return m_map.find(name)->second;
}
