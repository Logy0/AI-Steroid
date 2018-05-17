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
#include "CartPole.h"

using namespace std;

using namespace NEAT;


Population *pole2TestRealTime();

int pole2RealTimeLoop(Population *pop, CartPole *cart);

bool pole2Evaluate(Organism *org, CartPole *cart);

#endif
