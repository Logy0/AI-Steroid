#pragma once
#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include "Neat.h"
#include "Network.h"
#include "Population.h"
#include "Organism.h"
#include "Genome.h"
#include "Species.h"
#include "aisteroidgame.h"

using namespace std;

using namespace NEAT;


Population *aisteroidTestRealTime();

int aisteroidRealTimeLoop(Population *pop,Game *g);

bool aisteroidEvaluate(Organism *org,Game *g);

#endif
