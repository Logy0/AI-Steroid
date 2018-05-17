
#pragma once
//
// Created by ME on 28/03/18.
//
#ifndef UNICODE
#define UNICODE
#endif

#ifndef RTNEAT_GAME_H
#define RTNEAT_GAME_H

#include <cmath>
#include <time.h>
#include <stdio.h>
#include <array>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <headers/Defines.hpp>
#include <headers/AsteroidManager.hpp>
#include <headers/Asteroid.hpp>
#include <headers/TextureManager.hpp>
#include <headers/IAGridVisual.hpp>
#include <headers/Bullet.hpp>
#include <headers/Vessel.hpp>
#include <headers/CollisionsManager.hpp>
#include <headers/BulletManager.hpp>
#include <headers/Math.hpp>

#include "Level.h"



#include "Network.h"


using namespace std;
using namespace NEAT;

class Game {

public:
	Game();
	float gamma = 0.8; //change to 0.8-0.9
	virtual double evalNet(Network *net);
	int reset();
	int clear();
	int move(int action);
	double MAX_FITNESS;
	bool GENERALIZATION_TEST;               //Flag we are testing champ's generalization

	IAGridVisual grid;
	float oldgrid[GRID_Y][GRID_X];
	ofstream datafile;
	
	sf::RenderWindow window;
	sf::Clock clock;
	double scoreset[20000];
	int scoresize = 20000;
	int curset = 0;
	float sec;
	static const int n_ast = 1;
	float astpos[n_ast][2];
	float astspeed[n_ast][2];
	float meanradius[n_ast];
	float score;
	int fps,mode;
	float px, py, vx, vy;
	int x, y;
	int v_x,v_y;
	virtual void init();

	bool gameover;
	bool drawwin;
	bool display_grid;
protected:

private:
	TextureManager textureMgr;

	AsteroidManager manager = AsteroidManager(textureMgr);
	
	BulletManager bulletManager = BulletManager(textureMgr);
	
	Vessel vessel=Vessel(textureMgr, bulletManager, { WINDOW_X/2.0,WINDOW_Y / 2.0 });
	
	CollisionsManager collisionsManager=CollisionsManager(vessel, manager, bulletManager);
	
	int spawnAs();
	int spawnIt();
	uint8_t tdir = THRUST_DIRECTION::NONE;
	Level* level;
	bool isDead();
	void Game::subset(double *input, int v_x, int v_y, int n);
	const static int NUM_INPUTS;


};
#endif //RTNEAT_GAME_H