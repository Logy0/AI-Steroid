#include "../headers/IAGridVisual.hpp"

IAGridVisual::IAGridVisual()
{
    for( uint16_t i=0 ; i < GRID_Y ; i++ )
    {
        for( uint16_t j=0 ; j < GRID_X ; j++ )
        {
            grid_value[i][j] = 0;
            grid[i][j].setSize(sf::Vector2f(WINDOW_X/GRID_X,WINDOW_Y/GRID_Y));
            grid[i][j].setPosition(j*WINDOW_X/GRID_X,i*WINDOW_Y/GRID_Y);
            grid[i][j].setOutlineThickness(1);
            grid[i][j].setOutlineColor(sf::Color(128,128,128));
            grid[i][j].setFillColor(sf::Color::Transparent);
        }
    }
}

void IAGridVisual::updateVisual()
{
    for( uint16_t i=0 ; i < GRID_Y ; i++ )
    {
        for( uint16_t j=0 ; j < GRID_X ; j++ )
        {
            switch (grid_value[i][j])
            {

                case 1:
                    grid[i][j].setFillColor(sf::Color(255,0,0,80));
                    break;
                case 2:
                    grid[i][j].setFillColor(sf::Color(0,255,0,80));
                    break;
                case 3:
                    grid[i][j].setFillColor(sf::Color(0,0,255,80));
                    break;
                case 4:
                    grid[i][j].setFillColor(sf::Color(0,0,255,80));
                    break;
                default:
                    grid[i][j].setFillColor(sf::Color::Transparent);
                    break;
            }
        }
    }
}

void IAGridVisual::reset()
{
    for( uint16_t i=0 ; i < GRID_Y ; i++ )
    {
        for( uint16_t j=0 ; j < GRID_X ; j++ )
        {
            grid_value[i][j] = 0;
            grid[i][j].setFillColor(sf::Color::Transparent);
        }
    }
}

void IAGridVisual::update( const HasPhysics* object )
{
    int16_t pos_in_grid_x =  object->getPosition().x/WINDOW_X*GRID_X;
    int16_t pos_in_grid_y =  object->getPosition().y/WINDOW_Y*GRID_Y;
    if( pos_in_grid_x < GRID_X && pos_in_grid_y < GRID_Y && pos_in_grid_x >= 0 && pos_in_grid_y >= 0)
        grid_value[pos_in_grid_y][pos_in_grid_x] = object->getTypeId();
}
void IAGridVisual::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for( uint16_t i=0 ; i < GRID_Y ; i++ )
    {
        for( uint16_t j=0 ; j < GRID_X ; j++ )
        {
            target.draw(grid[i][j],states);
        }
    }
}
