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


#include "Network.h"


using namespace std;
using namespace NEAT;

class Game {

public:
	Game();
	float gamma = 0.7;
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
	static const int n_ast = 20;
	float astpos[n_ast][2];
	float astspeed[n_ast][2];
	float score;
	int fps;
	float px, py, vx, vy;
	int x, y;
	int v_x;
	int v_y;
	virtual void init();

	bool gameover;
	bool drawwin;

protected:

private:
	TextureManager textureMgr;
	AsteroidManager manager = AsteroidManager(textureMgr);
	bool isDead();
	void Game::subset(double *input, int v_x, int v_y, int n);
	const static int NUM_INPUTS;


};
#endif //RTNEAT_GAME_H