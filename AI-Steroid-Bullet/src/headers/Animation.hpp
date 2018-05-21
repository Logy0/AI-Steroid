//
// Created by asphox on 21/05/18.
//

#ifndef AI_STEROID_ANIMATION_HPP
#define AI_STEROID_ANIMATION_HPP

#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>

class Animation
{
private:
    std::thread thread;
    sf::Texture texture;
    sf::Shape& sprite;
    uint8_t index = 0;
    bool enable;
    bool active_thread = true;
public:
    Animation(const sf::Texture& texture,sf::Shape& sprite, bool enable = true):
            texture(texture),
            sprite(sprite),
            enable(enable)
    {
        sprite.setTexture(&texture);
        thread = std::thread(&Animation::tick,this);
    }

    ~Animation()
    {
        active_thread=false;
        thread.detach();
    }

    void tick()
    {
        while(active_thread)
        {
            if(enable)
            {
                sprite.setTextureRect(sf::IntRect(index*texture.getSize().x/4,0,texture.getSize().x/4,texture.getSize().y));
                index++;
                if(index == 4)index = 0;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
};

#endif //AI_STEROID_ANIMATION_HPP
