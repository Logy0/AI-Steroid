#include "experimentsaisteroid.h"
#include "aisteroidgame.h"

#include<sstream>
#include<iostream>
#include<cmath>
#include<Windows.h>
#include<ctime>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define NO_SCREEN_OUT
/**
* Real-time NEAT Validation
* Perform evolution on double pole balacing using rtNEAT methods calls
* Always uses Markov case (i.e. velocities provided)
* This test is meant to validate the rtNEAT methods and show how they can be used instead
* of the usual generational NEAT
* */
using namespace std;

struct thread_data
{
	HANDLE *threads;
	Population *pop;
	Organism **org;
	int m_id, n, counter, Size;
	int *check;
};
struct thread_id
{
	int id;
	int lenarray;
	bool status;
};

thread_data * pointer;
thread_id * id;



DWORD WINAPI thread_func(LPVOID lpParameter)
{
	thread_data *lp = (thread_data*)lpParameter;
	int m_id = (*lp).m_id;
	(*lp).m_id += 1;
	Organism *org;
	Population *pop = (*lp).pop;

	Game *g = new Game();
	g->init();
	SuspendThread(GetCurrentThread());
	while (1)
	{
		//org = (*lp).org[m_id];
		org= (pop->chooseParentSpecies())->reproduceOne(m_id, pop, pop->species);
		
		aisteroidEvaluate(org, g);

		//Now we reestimate the baby's species' fitness
		org->species->estimateAverage();
		//Remove the worst organism
		pop->removeWorst();


		cout << "score :" << g->score << endl;
		
		pointer->check[m_id] = 1;
		pointer->counter++;

		SuspendThread(GetCurrentThread());

	}
}
void InitializeThreads(HANDLE *threads, int n)
{
	pointer->m_id = 0;
	pointer->counter = 0;

	for (int k = 0;k<n;k++)
	{
		id[k].id = k;
		id[k].status = false;
		threads[k] = CreateThread(NULL, 0, thread_func, pointer, 0, 0);

		//while (pointer->check[k] != 1) {
		//	cout << " ";
		//}
		//cout << "" << endl;
	}
	while (pointer->counter<n - 1) {}
}

Population *aisteroidTestRealTime() {
	
	unsigned int seed = (unsigned)time(NULL);
	srand(seed);
	int mode;
	cout << "--------SELECTION MENU--------" << endl;
	cout << "1 : Play the game (you just lost) " << endl;
	cout << "2 : Train some IAs (no multithread) with SFML window" << endl;
	cout << "3 : Train some IAs (multithread / always experimental) without SFML window" << endl;
	cin >> mode;
	cout << "" << endl;
	Game *g = new Game();
	
	Population *pop = nullptr;
	switch (mode)
	{
		case 1:
			g->mode = 1;
			g->init();
			g->move(8);
			break;
		case 2:
		{
			g->mode = 2;
			g->init();
			cout << "START AI-STEROID REAL-TIME EVOLUTION VALIDATION" << endl;
			cout << "Reading in the start genome" << endl;
			char curWord[20];
			int id;
			ifstream iFile("aisteroidstartgenes1", ios::in);
			iFile >> curWord;
			iFile >> id;
			cout << "Reading in Genome id " << id << endl;
			Genome *startGenome = new Genome(id, iFile);
			iFile.close();
			cout << "Start Genome: " << startGenome << endl;
			cout << "Spawning Population off Genome" << endl;

			pop = new Population(startGenome, NEAT::popSize);
			cout << "New game" << endl;
			ofstream datafile;
			g->datafile.open("save.txt");
			g->datafile << seed << endl;
			g->datafile << "[";

			cout << "Verifying Spawned Pop" << endl;
			pop->verifyPopulation();
			cout << "REAL TIME LOOP" << endl;

			//Start the evolution loop using rtNEAT method calls 
			aisteroidRealTimeLoop(pop, g);
			g->datafile << "0]";
			g->datafile.close();
			return pop;
			break;
		}
			
		case 3:
		{
			g->mode = 3;
			cout << "START AI-STEROID REAL-TIME EVOLUTION VALIDATION" << endl;
			cout << "Reading in the start genome" << endl;
			char curWord[20];
			int id;
			ifstream iFile("aisteroidstartgenes1", ios::in);
			iFile >> curWord;
			iFile >> id;
			cout << "Reading in Genome id " << id << endl;
			Genome *startGenome = new Genome(id, iFile);
			iFile.close();
			cout << "Start Genome: " << startGenome << endl;
			cout << "Spawning Population off Genome" << endl;

			pop = new Population(startGenome, NEAT::popSize);
			cout << "New game" << endl;
			ofstream datafile;
			g->datafile.open("save.txt");
			g->datafile << seed << endl;
			g->datafile << "[";

			cout << "Verifying Spawned Pop" << endl;
			pop->verifyPopulation();
			cout << "REAL TIME LOOP" << endl;

			//Start the evolution loop using rtNEAT method calls 
			aisteroidRealTimeLoop(pop, g);
			g->datafile << "0]";
			g->datafile.close();
			return pop;
			break;
		}
		default:
			cout << "I'm sorry Dave, I'm afraid I can't do that" << endl;
			break;

	}
	
	return pop;
}

int aisteroidRealTimeLoop(Population *pop, Game *g) {
	int pause;
	bool win = false;
	//We try to keep the number of species constant at this number
	int numSpeciesTarget = NEAT::popSize / 15;

	//This is where we determine the frequency of compatibility threshold adjustment
	int compatAdjustFrequency = NEAT::popSize / 10;
	if (compatAdjustFrequency < 1)
		compatAdjustFrequency = 1;

	//Initially, we evaluate the whole population
	//Evaluate each organism on a test
	cout << "Evaluating the population" << endl;
	vector<Organism *>::iterator curOrg;

	for (curOrg = (pop->organisms).begin(); curOrg != (pop->organisms).end(); ++curOrg) {
		//shouldn't happen
		if (((*curOrg)->gnome) == 0) {
			cout << "ERROR EMPTY GENOME!" << endl;
			cin >> pause;
		}
		if (aisteroidEvaluate((*curOrg), g)) win = true;
	}
	//Get ready for real-time loop
	//Rank all the organisms from best to worst in each species
	pop->rankWithinSpecies();
	//Assign each species an average fitness
	//This average must be kept up-to-date by rtNEAT in order to select species probabailistically for reproduction
	pop->estimateAllAverages();
	//Now create offspring one at a time, testing each offspring,
	//and replacing the worst with the new offspring if its better
	for (int offspringCount = 0; offspringCount < 50000; offspringCount++) {
	
		cout << "Ite :" << offspringCount << endl;
		//Every popSize reproductions, adjust the compatThresh to better match the numSpeciesTarger
		//and reassign the population to new species
		if (offspringCount % compatAdjustFrequency == 0) {
			int numSpecies = (int)pop->species.size();
			double compatMod = 0.1;  //Modify compat thresh to control speciation(

										// This tinkers with the compatibility threshold
			if (numSpecies < numSpeciesTarget) {
				NEAT::compatThreshold -= compatMod;
			}
			else if (numSpecies > numSpeciesTarget)
				NEAT::compatThreshold += compatMod;

			if (NEAT::compatThreshold < 0.3)
				NEAT::compatThreshold = 0.3;

			//cout << "compatThreshold = " << NEAT::compatThreshold << endl;

			//Go through entire population, reassigning organisms to new species
			for (curOrg = (pop->organisms).begin(); curOrg != pop->organisms.end(); ++curOrg) {
				pop->reassignSpecies(*curOrg);
			}

		}

	Organism *newOrg = (pop->chooseParentSpecies())->reproduceOne(offspringCount, pop, pop->species);

	//cout << "Evaluating new baby: " << endl;
		
	if (aisteroidEvaluate(newOrg,g)) win = true;

	if (win) {
	cout << "WINNER" << endl;
	pop->printToFileBySpecies((char *) "rt_winpop");
	break;
	}


	//Now we reestimate the baby's species' fitness
	newOrg->species->estimateAverage();
	//Remove the worst organism
	pop->removeWorst();
		
	}

	return 0;
}

int aisteroidRealTimeLoopMT(Population *pop,Game *g) {
	int pause;
	bool win = false;
	HANDLE *threads;

	pointer = (thread_data*)new(thread_data);

	int n = 300;
	id = (thread_id*)new(thread_id[n]);

	threads = (HANDLE*)malloc(sizeof(HANDLE)*n);

	pointer->check = (int*)malloc(sizeof(int)*n);

	pointer->n = n;

	pointer->threads = threads;
	pointer->check[0] = 1;

	InitializeThreads(threads, n);

	pointer->org = (Organism**)malloc(sizeof(Organism*)*n);
	pointer->pop = pop;
	//We try to keep the number of species constant at this number
	int numSpeciesTarget = NEAT::popSize / 15;

	//This is where we determine the frequency of compatibility threshold adjustment
	int compatAdjustFrequency = NEAT::popSize / 10;
	if (compatAdjustFrequency < 1)
		compatAdjustFrequency = 1;

	//Initially, we evaluate the whole population
	//Evaluate each organism on a test
	cout << "Evaluating the population" << endl;
	vector<Organism *>::iterator curOrg;

	for (curOrg = (pop->organisms).begin(); curOrg != (pop->organisms).end(); ++curOrg) {
		//shouldn't happen
		if (((*curOrg)->gnome) == 0) {
			cout << "ERROR EMPTY GENOME!" << endl;
			cin >> pause;
		}
		if (aisteroidEvaluate((*curOrg),g)) win = true;
	}
	//Get ready for real-time loop
	//Rank all the organisms from best to worst in each species
	pop->rankWithinSpecies();
	//Assign each species an average fitness
	//This average must be kept up-to-date by rtNEAT in order to select species probabailistically for reproduction
	pop->estimateAllAverages();
	//Now create offspring one at a time, testing each offspring,
	//and replacing the worst with the new offspring if its better
	//for (int offspringCount = 0; offspringCount < 50000; offspringCount++) {
	for (int Gen = 0; Gen < n; Gen++) {

		for (int k = 0;k < pop->organisms.size();k++)
		{
			cout << "Gen :" << Gen << endl;
			//Every popSize reproductions, adjust the compatThresh to better match the numSpeciesTarger
			//and reassign the population to new species
			if (Gen*pop->organisms.size() % compatAdjustFrequency == 0) {
				int numSpecies = (int)pop->species.size();
				double compatMod = 0.1;  //Modify compat thresh to control speciation(

										 // This tinkers with the compatibility threshold
				if (numSpecies < numSpeciesTarget) {
					NEAT::compatThreshold -= compatMod;
				}
				else if (numSpecies > numSpeciesTarget)
					NEAT::compatThreshold += compatMod;

				if (NEAT::compatThreshold < 0.3)
					NEAT::compatThreshold = 0.3;

				//cout << "compatThreshold = " << NEAT::compatThreshold << endl;

				//Go through entire population, reassigning organisms to new species
				for (curOrg = (pop->organisms).begin(); curOrg != pop->organisms.end(); ++curOrg) {
					pop->reassignSpecies(*curOrg);
				}



			}

			


		}
			//aisteroidEvaluate(newOrg, g);
			//cout << "Evaluating new baby: " << endl;
			/*
			if (aisteroidEvaluate(newOrg,g)) win = true;

			if (win) {
			cout << "WINNER" << endl;
			pop->printToFileBySpecies((char *) "rt_winpop");
			break;
			}
			

			//Now we reestimate the baby's species' fitness
			newOrg->species->estimateAverage();
			//Remove the worst organism
			pop->removeWorst();
			*/
			//}
	}

	return 0;
}

bool aisteroidEvaluate(Organism *org, Game *g) {
	Network *net;

	int pause;

	net = org->net;
	
	org->fitness = g->evalNet(net);
	
	stringstream ss;
	ss << org->fitness;
	string str = ss.str();
	g->datafile << str << ",\n";
	

#ifndef NO_SCREEN_OUT
	if (org->popChampChild)
		cout << " <<DUPLICATE OF CHAMPION>> ";

	//Output to screen
	cout << "Org " << (org->gnome)->genomeId << " fitness: " << org->fitness;
	cout << " (" << (org->gnome)->genes.size();
	cout << " / " << (org->gnome)->nodes.size() << ")";
	cout << "   ";
	if (org->mutStructBaby) cout << " [struct]";
	if (org->mateBaby) cout << " [mate]";
	cout << endl;
#endif

	if (org->fitness >= (g->MAX_FITNESS - 1)) {
		org->winner = true;
		return true;
	}
	else {
		org->winner = false;
		return false;
	}
}
