#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <SFML/Graphics.hpp>

struct Math
{
    static inline float distance(const float x1,const float y1,const float x2,const float y2)
    {
        return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    }

    static inline float distance(const sf::Vector2f& a,const sf::Vector2f& b)
    {
        return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
    }

};

#endif //MATH_HPP
