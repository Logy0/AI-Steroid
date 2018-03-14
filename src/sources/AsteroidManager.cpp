#include "../headers/AsteroidManager.hpp"

AsteroidManager::AsteroidManager( const TextureManager& texturesMgr) :
texturesMgr(texturesMgr)
{
    dist_base_radius = std::normal_distribution<float>(0,0.1);
}

AsteroidManager::~AsteroidManager()
{
   clear();
}

Asteroid* AsteroidManager::generate(const sf::Vector2f& pos)
{
    float base_radius = MEAN_RADIUS_ASTEROIDS*(1+dist_base_radius(rand));

    if( base_radius > MEAN_RADIUS_ASTEROIDS*(1+MAX_MEAN_RADIUS_VARIATION_ASTEROIDS))
        base_radius = MEAN_RADIUS_ASTEROIDS*(1+MAX_MEAN_RADIUS_VARIATION_ASTEROIDS);
    else if( base_radius < MEAN_RADIUS_ASTEROIDS*(1-MIN_MEAN_RADIUS_VARIATION_ASTEROIDS) )
        base_radius = MEAN_RADIUS_ASTEROIDS*(1-MIN_MEAN_RADIUS_VARIATION_ASTEROIDS);

    dist_variations_radius = std::normal_distribution<float>(base_radius,ROUGHNESS_ASTEROIDS);
    std::array<sf::Vector2f,NB_OF_VERTEX_ASTEROIDS> vertices;
    float angle_increment = tau/NB_OF_VERTEX_ASTEROIDS;
    float radius = 0.0;
    for( size_t i=0 ; i<NB_OF_VERTEX_ASTEROIDS ; i++)
    {
        radius = dist_variations_radius(rand);
        vertices[i] = sf::Vector2f(radius*cos(i*angle_increment),radius*sin(i*angle_increment));
    }
    Asteroid* ast = new Asteroid(texturesMgr.get("asteroid"),base_radius,vertices,pos,{0,5},{0,0},0,10,0);
    list.insert(ast);
    return ast;
}

void AsteroidManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for( auto& it : list )
    target.draw(*it,states);
}

void AsteroidManager::clear()
{
    for( auto& it : list )
        delete it;
    list.clear();
}

void AsteroidManager::update(float timeElasped)
{
    for( auto& it : list )
        it->update(timeElasped);
}
