#include "experiments.h"
#define NO_SCREEN_OUT
/**
 * Real-time NEAT Validation
 * Perform evolution on double pole balacing using rtNEAT methods calls
 * Always uses Markov case (i.e. velocities provided)
 * This test is meant to validate the rtNEAT methods and show how they can be used instead
 * of the usual generational NEAT
 * */
Population *pole2TestRealTime() {
    cout << "START DOUBLE POLE BALANCING REAL-TIME EVOLUTION VALIDATION" << endl;
    cout << "Reading in the start genome" << endl;
    char curWord[20];
    int id;
    ifstream iFile("pole2startgenes1", ios::in);
    iFile >> curWord;
    iFile >> id;
    cout << "Reading in Genome id " << id << endl;
    Genome *startGenome = new Genome(id, iFile);
    iFile.close();
    cout << "Start Genome: " << startGenome << endl;
    cout << "Spawning Population off Genome" << endl;
    Population *pop = new Population(startGenome, NEAT::popSize);

    cout << "Verifying Spawned Pop" << endl;
    pop->verifyPopulation();

    CartPole *cartPole = new CartPole(1);

    //Start the evolution loop using rtNEAT method calls 
    pole2RealTimeLoop(pop, cartPole);

    return pop;
}

int pole2RealTimeLoop(Population *pop, CartPole *cart) {
    int pause;
    bool win = false;

    cart->N_MARKOV_LONG = false;
    cart->GENERALIZATION_TEST = false;

    //We try to keep the number of species constant at this number
    int numSpeciesTarget = NEAT::popSize / 15;

    //This is where we determine the frequency of compatibility threshold adjustment
    int compatAdjustFrequency = NEAT::popSize / 10;
    if (compatAdjustFrequency < 1)
        compatAdjustFrequency = 1;

    //Initially, we evaluate the whole population
    //Evaluate each organism on a test
    vector<Organism *>::iterator curOrg;
    for (curOrg = (pop->organisms).begin(); curOrg != (pop->organisms).end(); ++curOrg) {
        //shouldn't happen
        if (((*curOrg)->gnome) == 0) {
            cout << "ERROR EMPTY GENOME!" << endl;
            cin >> pause;
        }
        if (pole2Evaluate((*curOrg), cart)) win = true;
    }

    //Get ready for real-time loop
    //Rank all the organisms from best to worst in each species
    pop->rankWithinSpecies();

    //Assign each species an average fitness
    //This average must be kept up-to-date by rtNEAT in order to select species probabailistically for reproduction
    pop->estimateAllAverages();

    //Now create offspring one at a time, testing each offspring,
    //and replacing the worst with the new offspring if its better
    for (int offspringCount = 0; offspringCount < 20000; offspringCount++) {
        //Every popSize reproductions, adjust the compatThresh to better match the numSpeciesTarger
        //and reassign the population to new species
        if (offspringCount % compatAdjustFrequency == 0) {
            int numSpecies = (int) pop->species.size();
            double compatMod = 0.1;  //Modify compat thresh to control speciation

            // This tinkers with the compatibility threshold
            if (numSpecies < numSpeciesTarget) {
                NEAT::compatThreshold -= compatMod;
            } else if (numSpecies > numSpeciesTarget)
                NEAT::compatThreshold += compatMod;

            if (NEAT::compatThreshold < 0.3)
                NEAT::compatThreshold = 0.3;

            //cout << "compatThreshold = " << NEAT::compatThreshold << endl;

            //Go through entire population, reassigning organisms to new species
            for (curOrg = (pop->organisms).begin(); curOrg != pop->organisms.end(); ++curOrg) {
                pop->reassignSpecies(*curOrg);
            }
        }
		/*
        //For printing only
        vector<Species *>::iterator curSpec;
        for (curSpec = (pop->species).begin(); curSpec != (pop->species).end(); curSpec++) {
            cout << "Species " << (*curSpec)->id << " size" << (*curSpec)->organisms.size() << " average= "
                 << (*curSpec)->avgEst << endl;
        }

        cout << "Pop size: " << pop->organisms.size() << endl;
		*/
        //Here we call two rtNEAT calls:
        //1) chooseParentSpecies() decides which species should produce the next offspring
        //2) reproduceOne(...) creates a single offspring from the chosen species
        Organism *newOrg = (pop->chooseParentSpecies())->reproduceOne(offspringCount, pop, pop->species);

        //Now we evaluate the new individual
        //Note that in a true real-time simulation, evaluation would be happening to all individuals at all times.
        //That is, this call would not appear here in a true online simulation.
        //cout << "Evaluating new baby: " << endl;
        if (pole2Evaluate(newOrg, cart)) win = true;

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

bool pole2Evaluate(Organism *org, CartPole *cart) {
    Network *net;

    int pause;

    net = org->net;

    //Try to balance a pole now
    org->fitness = cart->evalNet(net);

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

    if ((!(cart->GENERALIZATION_TEST)) && (!(cart->N_MARKOV_LONG)))
        if (org->popChampChild) {
            cout << org->gnome << endl;
            //DEBUG CHECK
            if (org->highFit > org->fitness) {
                cout << "ALERT: ORGANISM DAMAGED" << endl;
                printGenomeToFile(org->gnome, "failure_champ_genome");
                cin >> pause;
            }
        }

    //Decide if its a winner, in Markov Case
    if (cart->MARKOV) {
        if (org->fitness >= (cart->MAX_FITNESS - 1)) {
            org->winner = true;
            return true;
        } else {
            org->winner = false;
            return false;
        }
    }
    //if doing the long test non-markov
    else if (cart->N_MARKOV_LONG) {
        if (org->fitness >= 100002) {//if (org->fitness >= 99999) {
            org->winner = true;
            return true;
        } else {
            org->winner = false;
            return false;
        }
    } else if (cart->GENERALIZATION_TEST) {
        if (org->fitness >= 100002) {// if (org->fitness >= 999) {
            org->winner = true;
            return true;
        } else {
            org->winner = false;
            return false;
        }
    } else {
        org->winner = false;
        return false;  //Winners not decided here in non-Markov
    }
}
