#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include  <SFML/Graphics.hpp>
#include <map>

class TextureManager
{
private:
    std::map<const std::string , sf::Texture > m_map;

public:
    void add( const std::string& name , const std::string& path );
    void add( const std::string& name , const std::string& path , const sf::IntRect& area );
    const sf::Texture& get( const std::string& name ) const;
};

#endif //TEXTURE_MANAGER_HPP
