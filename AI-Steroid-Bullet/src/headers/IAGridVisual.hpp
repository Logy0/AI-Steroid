#ifndef IAGRIDVISUAL_HPP
#define IAGRIDVISUAL_HPP

#include <set>
#include <SFML/Graphics.hpp>
#include "Defines.hpp"
#include "HasPhysics.hpp"

class IAGridVisual : public sf::Drawable
{
private:
    sf::RectangleShape grid[GRID_Y][GRID_X];
public:
    uint8_t grid_value[GRID_Y][GRID_X];
    
    IAGridVisual();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();
    void updateVisual();
    void update( const HasPhysics* );
};

#endif //IAGRIDVISUAL_HPP
