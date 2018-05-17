#include "Species.h"
#include <iostream>

using namespace NEAT;

Species::Species(int i) {
    id = i;
    age = 1;
    avgFitness = 0.0;
    expectedOffspring = 0;
    novel = false;
    ageOfLastImprovement = 0;
    maxFitness = 0;
    maxFitnessEver = 0;
    obliterate = false;

    avgEst = 0;
}

Species::Species(int i, bool n) {
    id = i;
    age = 1;
    avgFitness = 0.0;
    expectedOffspring = 0;
    novel = n;
    ageOfLastImprovement = 0;
    maxFitness = 0;
    maxFitnessEver = 0;
    obliterate = false;

    avgEst = 0;
}


Species::~Species() {

    std::vector<Organism *>::iterator curOrg;

    for (curOrg = organisms.begin(); curOrg != organisms.end(); ++curOrg) {
        delete (*curOrg);
    }

}

bool Species::rank() {
    std::sort(organisms.begin(), organisms.end(), order_orgs);
    return true;
}

double Species::estimateAverage() {
    std::vector<Organism *>::iterator curOrg;
    double total = 0.0; //running total of fitnesses

    //Note: Since evolution is happening in real-time, some organisms may not
    //have been around long enough to count them in the fitness evaluation

    double numOrgs = 0; //counts number of orgs above the time_alive threshold


    for (curOrg = organisms.begin(); curOrg != organisms.end(); ++curOrg) {
        //New variable time_alive
        if (((*curOrg)->timeAlive) >= NEAT::timeAliveMinimum) {
            total += (*curOrg)->fitness;
            ++numOrgs;
        }
    }

    if (numOrgs > 0)
        avgEst = total / numOrgs;
    else {
        avgEst = 0;
    }

    return avgEst;
}


Organism *Species::reproduceOne(int generation, Population *pop, std::vector<Species *> &sorted_species) {
    //bool Species::reproduce(int generation, Population *pop,std::vector<Species*> &sorted_species) {
    int count = generation; //This will assign genome id's according to the generation
    std::vector<Organism *>::iterator curorg;


    std::vector<Organism *> eligOrgs; //This list contains the eligible organisms (KEN)

    int poolSize;  //The number of Organisms in the old generation

    int orgNum;  //Random variable
    int orgCount;
    Organism *mom = 0; //Parent Organisms
    Organism *dad = 0;
    Organism *baby;  //The new Organism

    Genome *new_genome;  //For holding baby's genes

    std::vector<Species *>::iterator curspecies;  //For adding baby
    Species *newSpecies; //For babies in new Species
    Organism *compOrg;  //For Species determination through comparison

    Species *randSpecies;  //For mating outside the Species
    double randMult;
    int randSpeciesNum;
    int spCount;
    std::vector<Species *>::iterator curSp;

    Network *netAnalogue;  //For adding link to test for recurrency

    bool outside;

    bool found;  //When a Species is found

    Organism *thechamp;

    int giveUp; //For giving up finding a mate outside the species

    bool mutStructBaby;
    bool mateBaby;

    //The weight mutation power is species specific depending on its age
    double mutPower = NEAT::weightMutPower;

    //Roulette wheel variables
    double totalFitness = 0.0;
    double marble;  //The marble will have a number between 0 and total_fitness
    double spin;  //Fitness total while the wheel is spinning

    //Check for a mistake
    if ((organisms.size() == 0)) {
        return false;
    }

    rank(); //Make sure organisms are ordered by rank

    //ADDED CODE (Ken)
    //Now transfer the list to elig_orgs without including the ones that are too young (Ken)
    for (curorg = organisms.begin(); curorg != organisms.end(); ++curorg) {
        if ((*curorg)->timeAlive >= NEAT::timeAliveMinimum)
            eligOrgs.push_back(*curorg);
    }

    //Now elig_orgs should be an ordered list of mature organisms
    //Special case: if it's empty, then just include all the organisms (age doesn't matter in this case) (Ken)
    if (eligOrgs.size() == 0) {
        for (curorg = organisms.begin(); curorg != organisms.end(); ++curorg) {
            eligOrgs.push_back(*curorg);
        }
    }

    //Now elig_orgs is guaranteed to contain either an ordered list of mature orgs or all the orgs (Ken)
    //We may also want to check to see if we are getting pools of >1 organism (to make sure our survivalThresh is sensible) (Ken)

    //Only choose from among the top ranked orgs
    poolSize = (int) ((eligOrgs.size() - 1) * NEAT::survivalThresh);

    //Compute total fitness of species for a roulette wheel
    //Note: You don't get much advantage from a roulette here
    // because the size of a species is relatively small.
    // But you can use it by using the roulette code here
    for (curorg = eligOrgs.begin(); curorg != eligOrgs.end(); ++curorg) {
        totalFitness += (*curorg)->fitness;
    }

    //In reproducing only one offspring, the champ shouldn't matter
    //thechamp=(*(organisms.begin()));

    //Create one offspring for the Species

    mutStructBaby = false;
    mateBaby = false;

    outside = false;

    //First, decide whether to mate or mutate
    //If there is only one organism in the pool, then always mutate
    if ((randFloat() < NEAT::mutateOnlyProb) ||
        poolSize == 0) {

        //Choose the random parent

        //RANDOM PARENT CHOOSER
        orgNum = randInt(0, poolSize);
        curorg = eligOrgs.begin();
        for (orgCount = 0; orgCount < orgNum; orgCount++)
            ++curorg;



        ////Roulette Wheel
        //marble=randFloat()*total_fitness;
        //curorg=elig_orgs.begin();
        //spin=(*curorg)->fitness;
        //while(spin<marble) {
        //	++curorg;

        //Keep the wheel spinning
        //	spin+=(*curorg)->fitness;
        //}
        //Finished roulette


        mom = (*curorg);

        new_genome = (mom->gnome)->duplicate(count);

        //Do the mutation depending on probabilities of
        //various mutations

        if (randFloat() < NEAT::mutateAddNodeProb) {
            //cout<<"mutate add node"<<endl;
            new_genome->mutateAddNode(pop->innovations, pop->curNodeId, pop->curInnovNum);
            mutStructBaby = true;
        } else if (randFloat() < NEAT::mutateAddLinkProb) {
            //cout<<"mutate add link"<<endl;
            netAnalogue = new_genome->genesis(generation);
            new_genome->mutateAddLink(pop->innovations, pop->curInnovNum, NEAT::newLinkTries);
            delete netAnalogue;
            mutStructBaby = true;
        }
            //NOTE:  A link CANNOT be added directly after a node was added because the phenotype
            //       will not be appropriately altered to reflect the change
        else {
            //If we didn't do a structural mutation, we do the other kinds

            if (randFloat() < NEAT::mutateRandomTraitProb) {
                //cout<<"mutate random trait"<<endl;
                new_genome->mutateRandomTrait();
            }
            if (randFloat() < NEAT::mutateLinkTraitProb) {
                //cout<<"mutate_link_trait"<<endl;
                new_genome->mutateLinkTrait(1);
            }
            if (randFloat() < NEAT::mutateNodeTraitProb) {
                //cout<<"mutate_node_trait"<<endl;
                new_genome->mutateNodeTrait(1);
            }
            if (randFloat() < NEAT::mutateLinkWeightsProb) {
                //cout<<"mutate_link_weights"<<endl;
                new_genome->mutateLinkWeights(mutPower, 1.0, GAUSSIAN);
            }
            if (randFloat() < NEAT::mutateToggleEnableProb) {
                //cout<<"mutate toggle enable"<<endl;
                new_genome->mutateToggleEnable(1);

            }
            if (randFloat() < NEAT::mutateGeneReEnableProb) {
                //cout<<"mutate gene reenable"<<endl;
                new_genome->mutateGeneReEnable();
            }
        }

        baby = new Organism(0.0, new_genome, generation);

    }

        //Otherwise we should mate
    else {

        //Choose the random mom
        orgNum = randInt(0, poolSize);
        curorg = eligOrgs.begin();
        for (orgCount = 0; orgCount < orgNum; orgCount++)
            ++curorg;


        ////Roulette Wheel
        //marble=randFloat()*total_fitness;
        //curorg=elig_orgs.begin();
        //spin=(*curorg)->fitness;
        //while(spin<marble) {
        //	++curorg;

        //Keep the wheel spinning
        //	spin+=(*curorg)->fitness;
        //}
        //Finished roulette


        mom = (*curorg);

        //Choose random dad

        if ((randFloat() > NEAT::interSpeciesMateRate)) {
            //Mate within Species

            orgNum = randInt(0, poolSize);
            curorg = eligOrgs.begin();
            for (orgCount = 0; orgCount < orgNum; orgCount++)
                ++curorg;


            ////Use a roulette wheel
            //marble=randFloat()*total_fitness;
            //curorg=elig_orgs.begin();
            //spin=(*curorg)->fitness;
            //while(spin<marble) {
            //	++curorg;


            //Keep the wheel spinning
            //	spin+=(*curorg)->fitness;
            //}
            ////Finished roulette


            dad = (*curorg);
        } else {

            //Mate outside Species
            randSpecies = this;

            //Select a random species
            giveUp = 0;  //Give up if you cant find a different Species
            while ((randSpecies == this) && (giveUp < 5)) {

                //This old way just chose any old species
                //randspeciesnum=randInt(0,(pop->species).size()-1);

                //Choose a random species tending towards better species
                randMult = gaussRand() / 4;
                if (randMult > 1.0) randMult = 1.0;
                //This tends to select better species
                randSpeciesNum = (int) floor((randMult * (sorted_species.size() - 1.0)) + 0.5);
                curSp = (sorted_species.begin());
                for (spCount = 0; spCount < randSpeciesNum; spCount++)
                    ++curSp;
                randSpecies = (*curSp);

                ++giveUp;
            }

            //OLD WAY: Choose a random dad from the random species
            //Select a random dad from the random Species
            //NOTE:  It is possible that a mating could take place
            //       here between the mom and a baby from the NEW
            //       generation in some other Species
            //orgnum=randInt(0,(randspecies->organisms).size()-1);
            //curorg=(randspecies->organisms).begin();
            //for(orgcount=0;orgcount<orgnum;orgcount++)
            //  ++curorg;
            //dad=(*curorg);

            //New way: Make dad be a champ from the random species
            dad = (*((randSpecies->organisms).begin()));

            outside = true;
        }

        //Perform mating based on probabilities of differrent mating types
        if (randFloat() < NEAT::mateMultiPointProb) {
            new_genome = (mom->gnome)->mateMultiPoint(dad->gnome, count, mom->origFitness, dad->origFitness, outside);
        } else if (randFloat() <
                   (NEAT::mateMultiPointAvgProb / (NEAT::mateMultiPointAvgProb + NEAT::mateSinglePointProb))) {
            new_genome = (mom->gnome)->mateMultiPointAvg(dad->gnome, count, mom->origFitness, dad->origFitness,
                                                         outside);
        } else {
            new_genome = (mom->gnome)->mateSinglePoint(dad->gnome, count);
        }

        mateBaby = true;

        //Determine whether to mutate the baby's Genome
        //This is done randomly or if the mom and dad are the same organism
        if ((randFloat() > NEAT::mateOnlyProb) ||
            ((dad->gnome)->genomeId == (mom->gnome)->genomeId) ||
            (((dad->gnome)->compatibility(mom->gnome)) == 0.0)) {

            //Do the mutation depending on probabilities of
            //various mutations
            if (randFloat() < NEAT::mutateAddNodeProb) {
                new_genome->mutateAddNode(pop->innovations, pop->curNodeId, pop->curInnovNum);
                //  cout<<"mutate_add_node: "<<new_genome<<endl;
                mutStructBaby = true;
            } else if (randFloat() < NEAT::mutateAddLinkProb) {
                netAnalogue = new_genome->genesis(generation);
                new_genome->mutateAddLink(pop->innovations, pop->curInnovNum, NEAT::newLinkTries);
                delete netAnalogue;
                //cout<<"mutate_add_link: "<<new_genome<<endl;
                mutStructBaby = true;
            } else {
                //Only do other mutations when not doing strurctural mutations

                if (randFloat() < NEAT::mutateRandomTraitProb) {
                    new_genome->mutateRandomTrait();
                    //cout<<"..mutate random trait: "<<new_genome<<endl;
                }
                if (randFloat() < NEAT::mutateLinkTraitProb) {
                    new_genome->mutateLinkTrait(1);
                    //cout<<"..mutate link trait: "<<new_genome<<endl;
                }
                if (randFloat() < NEAT::mutateNodeTraitProb) {
                    new_genome->mutateNodeTrait(1);
                    //cout<<"mutate_node_trait: "<<new_genome<<endl;
                }
                if (randFloat() < NEAT::mutateLinkWeightsProb) {
                    new_genome->mutateLinkWeights(mutPower, 1.0, GAUSSIAN);
                    //cout<<"mutate_link_weights: "<<new_genome<<endl;
                }
                if (randFloat() < NEAT::mutateToggleEnableProb) {
                    new_genome->mutateToggleEnable(1);
                    //cout<<"mutate_toggle_enable: "<<new_genome<<endl;
                }
                if (randFloat() < NEAT::mutateGeneReEnableProb) {
                    new_genome->mutateGeneReEnable();
                    //cout<<"mutate_gene_reenable: "<<new_genome<<endl;
                }
            }

            //Create the baby
            baby = new Organism(0.0, new_genome, generation);

        } else {
            //Create the baby without mutating first
            baby = new Organism(0.0, new_genome, generation);
        }

    }

    //Add the baby to its proper Species
    //If it doesn't fit a Species, create a new one

    baby->mutStructBaby = mutStructBaby;
    baby->mateBaby = mateBaby;

    curspecies = (pop->species).begin();
    if (curspecies == (pop->species).end()) {
        //Create the first species
        newSpecies = new Species(++(pop->lastSpecies), true);
        (pop->species).push_back(newSpecies);
        newSpecies->addOrganism(baby);  //Add the baby
        baby->species = newSpecies;  //Point the baby to its species
    } else {
        compOrg = (*curspecies)->first();
        found = false;


        // Testing out what happens when speciation is disabled
        //found = true;
        //(*curspecies)->add_Organism(baby);
        //baby->species = (*curspecies);


        while ((curspecies != (pop->species).end()) && (!found)) {
            if (compOrg == 0) {
                //Keep searching for a matching species
                ++curspecies;
                if (curspecies != (pop->species).end())
                    compOrg = (*curspecies)->first();
            } else if (((baby->gnome)->compatibility(compOrg->gnome)) < NEAT::compatThreshold) {
                //Found compatible species, so add this organism to it
                (*curspecies)->addOrganism(baby);
                baby->species = (*curspecies);  //Point organism to its species
                found = true;  //Note the search is over
            } else {
                //Keep searching for a matching species
                ++curspecies;
                if (curspecies != (pop->species).end())
                    compOrg = (*curspecies)->first();
            }
        }

        //If we didn't find a match, create a new species
        if (found == false) {
            newSpecies = new Species(++(pop->lastSpecies), true);
            (pop->species).push_back(newSpecies);
            newSpecies->addOrganism(baby);  //Add the baby
            baby->species = newSpecies;  //Point baby to its species
        }

    } //end else

    //Put the baby also in the master organism list
    (pop->organisms).push_back(baby);

    return baby; //Return a pointer to the baby
}

bool Species::addOrganism(Organism *o) {
    organisms.push_back(o);
    return true;
}

Organism *Species::get_champ() {
    double champ_fitness = -1.0;
    Organism *thechamp=NULL;
    std::vector<Organism *>::iterator curorg;

    for (curorg = organisms.begin(); curorg != organisms.end(); ++curorg) {
        //TODO: Remove DEBUG code
        //cout<<"searching for champ...looking at org "<<(*curorg)->gnome->genome_id<<" fitness: "<<(*curorg)->fitness<<endl;
        if (((*curorg)->fitness) > champ_fitness) {
            thechamp = (*curorg);
            champ_fitness = thechamp->fitness;
        }
    }

    //cout<<"returning champ #"<<thechamp->gnome->genome_id<<endl;

    return thechamp;

}

bool Species::removeOrg(Organism *org) {
    std::vector<Organism *>::iterator curorg;

    curorg = organisms.begin();
    while ((curorg != organisms.end()) &&
           ((*curorg) != org))
        ++curorg;

    if (curorg == organisms.end()) {
        //cout<<"ALERT: Attempt to remove nonexistent Organism from Species"<<endl;
        return false;
    } else {
        organisms.erase(curorg);
        return true;
    }

}

Organism *Species::first() {
    return *(organisms.begin());
}
/*
bool Species::print_to_file(std::ostream &outFile) {
	std::vector<Organism*>::iterator curorg;

	//Print a comment on the Species info
	//outFile<<endl<<"/* Species #"<<id<<" : (Size "<<organisms.size()<<") (AF "<<ave_fitness<<") (Age "<<age<<")  *///"<<endl<<endl;
//char tempbuf[1024];
//sprintf(tempbuf, sizeof(tempbuf), "/* Species #%d : (Size %d) (AF %f) (Age %d)  */\n\n", id, organisms.size(), average_est, age);
//sprintf(tempbuf, sizeof(tempbuf), "/* Species #%d : (Size %d) (AF %f) (Age %d)  */\n\n", id, organisms.size(), ave_fitness, age);
//outFile.write(strlen(tempbuf), tempbuf);

//Show user what's going on
//cout<<endl<<"/* Species #"<<id<<" : (Size "<<organisms.size()<<") (AF "<<ave_fitness<<") (Age "<<age<<")  */"<<endl;

//Print all the Organisms' Genomes to the outFile
//for(curorg=organisms.begin();curorg!=organisms.end();++curorg) {

//Put the fitness for each organism in a comment
//outFile<<endl<<"/* Organism #"<<((*curorg)->gnome)->genome_id<<" Fitness: "<<(*curorg)->fitness<<" Error: "<<(*curorg)->error<<" */"<<endl;

//	char tempbuf2[1024];
//	sprintf(tempbuf2, sizeof(tempbuf2), "/* Organism #%d Fitness: %f Error: %f */\n", ((*curorg)->gnome)->genome_id, (*curorg)->fitness, (*curorg)->error);
//	outFile.write(strlen(tempbuf2), tempbuf2);

//If it is a winner, mark it in a comment
//	if ((*curorg)->winner) {
//		char tempbuf3[1024];
//		sprintf(tempbuf3, sizeof(tempbuf3), "/* ##------$ WINNER %d SPECIES #%d $------## */\n", ((*curorg)->gnome)->genome_id, id);
//outFile<<"/* ##------$ WINNER "<<((*curorg)->gnome)->genome_id<<" SPECIES #"<<id<<" $------## */"<<endl;
//	}

//	((*curorg)->gnome)->print_to_file(outFile);
//We can confirm by writing the genome #'s to the screen
//cout<<((*curorg)->gnome)->genome_id<<endl;
//}

//return true;

//}*/

//Print Species to a file outFile
bool Species::printToFile(std::ofstream &outFile) {
    std::vector<Organism *>::iterator curorg;

    //Print a comment on the Species info
    outFile << std::endl << "/* Species #" << id << " : (Size " << organisms.size() << ") (AF " << avgFitness
            << ") (Age " << age << ")  */" << std::endl << std::endl;

    //Show user what's going on
    std::cout << std::endl << "/* Species #" << id << " : (Size " << organisms.size() << ") (AF " << avgFitness
              << ") (Age " << age << ")  */" << std::endl;

    //Print all the Organisms' Genomes to the outFile
    for (curorg = organisms.begin(); curorg != organisms.end(); ++curorg) {

        //Put the fitness for each organism in a comment
        outFile << std::endl << "/* Organism #" << ((*curorg)->gnome)->genomeId << " Fitness: " << (*curorg)->fitness
                << " Error: " << (*curorg)->error << " */" << std::endl;

        //If it is a winner, mark it in a comment
        if ((*curorg)->winner)
            outFile << "/* ##------$ WINNER " << ((*curorg)->gnome)->genomeId << " SPECIES #" << id << " $------## */"
                    << std::endl;

        ((*curorg)->gnome)->printToFile(outFile);
        //We can confirm by writing the genome #'s to the screen
        //std::cout<<((*curorg)->gnome)->genome_id<<std::endl;
    }

    return true;

}


bool Species::printToFile(std::ostream &outFile) {
    std::vector<Organism *>::iterator curorg;

    //Print a comment on the Species info
    //outFile<<std::endl<<"/* Species #"<<id<<" : (Size "<<organisms.size()<<") (AF "<<ave_fitness<<") (Age "<<age<<")  */"<<std::endl<<std::endl;
    char tempbuf[1024];
    sprintf_s(tempbuf, "/* Species #%d : (Size %d) (AF %f) (Age %d)  */\n\n", id, organisms.size(), avgEst, age);
    //sprintf(tempbuf, "/* Species #%d : (Size %d) (AF %f) (Age %d)  */\n\n", id, organisms.size(), ave_fitness, age);
    outFile << tempbuf;

    //Show user what's going on
    //std::cout<<std::endl<<"/* Species #"<<id<<" : (Size "<<organisms.size()<<") (AF "<<ave_fitness<<") (Age "<<age<<")  */"<<std::endl;

    //Print all the Organisms' Genomes to the outFile
    for (curorg = organisms.begin(); curorg != organisms.end(); ++curorg) {

        //Put the fitness for each organism in a comment
        //outFile<<std::endl<<"/* Organism #"<<((*curorg)->gnome)->genome_id<<" Fitness: "<<(*curorg)->fitness<<" Error: "<<(*curorg)->error<<" */"<<std::endl;
        char tempbuf2[1024];
        sprintf_s(tempbuf2, "/* Organism #%d Fitness: %f Time: %d */\n", ((*curorg)->gnome)->genomeId, (*curorg)->fitness,
                (*curorg)->timeAlive);
        outFile << tempbuf2;

        //If it is a winner, mark it in a comment
        if ((*curorg)->winner) {
            char tempbuf3[1024];
            sprintf_s(tempbuf3, "/* ##------$ WINNER %d SPECIES #%d $------## */\n", ((*curorg)->gnome)->genomeId, id);
            //outFile<<"/* ##------$ WINNER "<<((*curorg)->gnome)->genome_id<<" SPECIES #"<<id<<" $------## */"<<std::endl;
        }

        ((*curorg)->gnome)->printToFile(outFile);
        //We can confirm by writing the genome #'s to the screen
        //std::cout<<((*curorg)->gnome)->genome_id<<std::endl;
    }
    char tempbuf4[1024];
    sprintf_s(tempbuf4, "\n\n");
    outFile << tempbuf4;

    return true;

}


//Prints the champions of each species to files    
//starting with directory_prefix
//The file name are as follows: [prefix]g[generation_num]cs[species_num]
//Thus, they can be indexed by generation or species
//bool Population::print_species_champs_tofiles(char *directory_prefix, int generation) {
//
//ostrstream *fnamebuf; //File for output
//std::vector<Species*>::iterator curspecies;
//Organism *champ;
//int pause;
//
//std::cout<<generation<<std::endl;
//std::cout<<"Printing species champs to file"<<std::endl;
////cin>>pause;
//
////Step through the Species and print their champs to files
//for(curspecies=species.begin();curspecies!=species.end();++curspecies) {
//
//std::cout<<"Printing species "<<(*curspecies)->id<<" champ to file"<<std::endl;
//
////cin>>pause;
//
////Get the champ of this species
//champ=(*curspecies)->get_champ();
//
////Revise the file name
//fnamebuf=new ostrstream();
//(*fnamebuf)<<directory_prefix<<"g"<<generation<<"cs"<<(*curspecies)->id<<ends;  //needs end marker
//
////Print to file using organism printing (includes comments)
//champ->print_to_file(fnamebuf->str());
//
////Reset the name
//fnamebuf->clear();
//delete fnamebuf;
//}
//return true;
//}

double Species::computeMaxFitness() {
    double max = 0.0;
    std::vector<Organism *>::iterator curorg;

    for (curorg = organisms.begin(); curorg != organisms.end(); ++curorg) {
        if (((*curorg)->fitness) > max)
            max = (*curorg)->fitness;
    }

    maxFitness = max;

    return max;
}

bool Species::reproduce(int generation, Population *pop, std::vector<Species *> &sorted_species) {
    int count;
    std::vector<Organism *>::iterator curorg;

    int poolsize;  //The number of Organisms in the old generation

    int orgnum;  //Random variable
    int orgcount;
    Organism *mom; //Parent Organisms
    Organism *dad;
    Organism *baby;  //The new Organism

    Genome *new_genome;  //For holding baby's genes

    std::vector<Species *>::iterator curspecies;  //For adding baby
    Species *newspecies; //For babies in new Species
    Organism *comporg;  //For Species determination through comparison

    Species *randspecies;  //For mating outside the Species
    double randmult;
    int randspeciesnum;
    int spcount;
    std::vector<Species *>::iterator cursp;

    Network *net_analogue;  //For adding link to test for recurrency
    int pause;

    bool outside;

    bool found;  //When a Species is found

    bool champ_done = false; //Flag the preservation of the champion

    Organism *thechamp;

    int giveup; //For giving up finding a mate outside the species

    bool mut_struct_baby;
    bool mate_baby;

    //The weight mutation power is species specific depending on its age
    double mut_power = NEAT::weightMutPower;

    //Roulette wheel variables
    double total_fitness = 0.0;
    double marble;  //The marble will have a number between 0 and total_fitness
    double spin;  //0Fitness total while the wheel is spinning

    //Compute total fitness of species for a roulette wheel
    //Note: You don't get much advantage from a roulette here
    // because the size of a species is relatively small.
    // But you can use it by using the roulette code here
    //for(curorg=organisms.begin();curorg!=organisms.end();++curorg) {
    //  total_fitness+=(*curorg)->fitness;
    //}


    //Check for a mistake
    if ((expectedOffspring > 0) &&
        (organisms.size() == 0)) {
        //    std::cout<<"ERROR:  ATTEMPT TO REPRODUCE OUT OF EMPTY SPECIES"<<std::endl;
        return false;
    }

    poolsize = organisms.size() - 1;

    thechamp = (*(organisms.begin()));

    //Create the designated number of offspring for the Species
    //one at a time
    for (count = 0; count < expectedOffspring; count++) {

        mut_struct_baby = false;
        mate_baby = false;

        outside = false;

        //Debug Trap
        if (expectedOffspring > NEAT::popSize) {
            //      std::cout<<"ALERT: EXPECTED OFFSPRING = "<<expected_offspring<<std::endl;
            //      cin>>pause;
        }

        //If we have a super_champ (Population champion), finish off some special clones
        if ((thechamp->superChampOffspring) > 0) {
            mom = thechamp;
            new_genome = (mom->gnome)->duplicate(count);

            if ((thechamp->superChampOffspring) == 1) {

            }

            //Most superchamp offspring will have their connection weights mutated only
            //The last offspring will be an exact duplicate of this super_champ
            //Note: Superchamp offspring only occur with stolen babies!
            //      Settings used for published experiments did not use this
            if ((thechamp->superChampOffspring) > 1) {
                if ((randFloat() < 0.8) ||
                    (NEAT::mutateAddLinkProb == 0.0))
                    //ABOVE LINE IS FOR:
                    //Make sure no links get added when the system has link adding disabled
                    new_genome->mutateLinkWeights(mut_power, 1.0, GAUSSIAN);
                else {
                    //Sometimes we add a link to a superchamp
                    net_analogue = new_genome->genesis(generation);
                    new_genome->mutateAddLink(pop->innovations, pop->curInnovNum, NEAT::newLinkTries);
                    delete net_analogue;
                    mut_struct_baby = true;
                }
            }

            baby = new Organism(0.0, new_genome, generation);

            if ((thechamp->superChampOffspring) == 1) {
                if (thechamp->popChamp) {
                    //std::cout<<"The new org baby's genome is "<<baby->gnome<<std::endl;
                    baby->popChampChild = true;
                    baby->highFit = mom->origFitness;
                }
            }

            thechamp->superChampOffspring--;
        }
            //If we have a Species champion, just clone it
        else if ((!champ_done) &&
                 (expectedOffspring > 5)) {

            mom = thechamp; //Mom is the champ

            new_genome = (mom->gnome)->duplicate(count);

            baby = new Organism(0.0, new_genome, generation);  //Baby is just like mommy

            champ_done = true;

        }
            //First, decide whether to mate or mutate
            //If there is only one organism in the pool, then always mutate
        else if ((randFloat() < NEAT::mutateOnlyProb) ||
                 poolsize == 0) {

            //Choose the random parent

            //RANDOM PARENT CHOOSER
            orgnum = randInt(0, poolsize);
            curorg = organisms.begin();
            for (orgcount = 0; orgcount < orgnum; orgcount++)
                ++curorg;



            ////Roulette Wheel
            //marble=randFloat()*total_fitness;
            //curorg=organisms.begin();
            //spin=(*curorg)->fitness;
            //while(spin<marble) {
            //++curorg;

            ////Keep the wheel spinning
            //spin+=(*curorg)->fitness;
            //}
            ////Finished roulette
            //

            mom = (*curorg);

            new_genome = (mom->gnome)->duplicate(count);

            //Do the mutation depending on probabilities of
            //various mutations

            if (randFloat() < NEAT::mutateAddNodeProb) {
                //std::cout<<"mutate add node"<<std::endl;
                new_genome->mutateAddNode(pop->innovations, pop->curNodeId, pop->curInnovNum);
                mut_struct_baby = true;
            } else if (randFloat() < NEAT::mutateAddLinkProb) {
                //std::cout<<"mutate add link"<<std::endl;
                net_analogue = new_genome->genesis(generation);
                new_genome->mutateAddLink(pop->innovations, pop->curInnovNum, NEAT::newLinkTries);
                delete net_analogue;
                mut_struct_baby = true;
            }
                //NOTE:  A link CANNOT be added directly after a node was added because the phenotype
                //       will not be appropriately altered to reflect the change
            else {
                //If we didn't do a structural mutation, we do the other kinds

                if (randFloat() < NEAT::mutateRandomTraitProb) {
                    //std::cout<<"mutate random trait"<<std::endl;
                    new_genome->mutateRandomTrait();
                }
                if (randFloat() < NEAT::mutateLinkTraitProb) {
                    //std::cout<<"mutate_link_trait"<<std::endl;
                    new_genome->mutateLinkTrait(1);
                }
                if (randFloat() < NEAT::mutateNodeTraitProb) {
                    //std::cout<<"mutate_node_trait"<<std::endl;
                    new_genome->mutateNodeTrait(1);
                }
                if (randFloat() < NEAT::mutateLinkWeightsProb) {
                    //std::cout<<"mutate_link_weights"<<std::endl;
                    new_genome->mutateLinkWeights(mut_power, 1.0, GAUSSIAN);
                }
                if (randFloat() < NEAT::mutateToggleEnableProb) {
                    //std::cout<<"mutate toggle enable"<<std::endl;
                    new_genome->mutateToggleEnable(1);

                }
                if (randFloat() < NEAT::mutateGeneReEnableProb) {
                    //std::cout<<"mutate gene reenable"<<std::endl;
                    new_genome->mutateGeneReEnable();
                }
            }

            baby = new Organism(0.0, new_genome, generation);

        }

            //Otherwise we should mate
        else {

            //Choose the random mom
            orgnum = randInt(0, poolsize);
            curorg = organisms.begin();
            for (orgcount = 0; orgcount < orgnum; orgcount++)
                ++curorg;


            ////Roulette Wheel
            //marble=randFloat()*total_fitness;
            //curorg=organisms.begin();
            //spin=(*curorg)->fitness;
            //while(spin<marble) {
            //++curorg;

            ////Keep the wheel spinning
            //spin+=(*curorg)->fitness;
            //}
            ////Finished roulette
            //

            mom = (*curorg);

            //Choose random dad

            if ((randFloat() > NEAT::interSpeciesMateRate)) {
                //Mate within Species

                orgnum = randInt(0, poolsize);
                curorg = organisms.begin();
                for (orgcount = 0; orgcount < orgnum; orgcount++)
                    ++curorg;


                ////Use a roulette wheel
                //marble=randFloat()*total_fitness;
                //curorg=organisms.begin();
                //spin=(*curorg)->fitness;
                //while(spin<marble) {
                //++curorg;
                //}

                ////Keep the wheel spinning
                //spin+=(*curorg)->fitness;
                //}
                ////Finished roulette
                //

                dad = (*curorg);
            } else {

                //Mate outside Species
                randspecies = this;

                //Select a random species
                giveup = 0;  //Give up if you cant find a different Species
                while ((randspecies == this) && (giveup < 5)) {

                    //This old way just chose any old species
                    //randspeciesnum=randInt(0,(pop->species).size()-1);

                    //Choose a random species tending towards better species
                    randmult = gaussRand() / 4;
                    if (randmult > 1.0) randmult = 1.0;
                    //This tends to select better species
                    randspeciesnum = (int) floor((randmult * (sorted_species.size() - 1.0)) + 0.5);
                    cursp = (sorted_species.begin());
                    for (spcount = 0; spcount < randspeciesnum; spcount++)
                        ++cursp;
                    randspecies = (*cursp);

                    ++giveup;
                }

                //OLD WAY: Choose a random dad from the random species
                //Select a random dad from the random Species
                //NOTE:  It is possible that a mating could take place
                //       here between the mom and a baby from the NEW
                //       generation in some other Species
                //orgnum=randInt(0,(randspecies->organisms).size()-1);
                //curorg=(randspecies->organisms).begin();
                //for(orgcount=0;orgcount<orgnum;orgcount++)
                //  ++curorg;
                //dad=(*curorg);

                //New way: Make dad be a champ from the random species
                dad = (*((randspecies->organisms).begin()));

                outside = true;
            }

            //Perform mating based on probabilities of differrent mating types
            if (randFloat() < NEAT::mateMultiPointProb) {
                new_genome = (mom->gnome)->mateMultiPoint(dad->gnome, count, mom->origFitness, dad->origFitness,
                                                          outside);
            } else if (randFloat() < (NEAT::mateMultiPointAvgProb /
                                      (NEAT::mateMultiPointAvgProb + NEAT::mateSinglePointProb))) {
                new_genome = (mom->gnome)->mateMultiPointAvg(dad->gnome, count, mom->origFitness, dad->origFitness,
                                                             outside);
            } else {
                new_genome = (mom->gnome)->mateSinglePoint(dad->gnome, count);
            }

            mate_baby = true;

            //Determine whether to mutate the baby's Genome
            //This is done randomly or if the mom and dad are the same organism
            if ((randFloat() > NEAT::mateOnlyProb) ||
                ((dad->gnome)->genomeId == (mom->gnome)->genomeId) ||
                (((dad->gnome)->compatibility(mom->gnome)) == 0.0)) {

                //Do the mutation depending on probabilities of
                //various mutations
                if (randFloat() < NEAT::mutateAddNodeProb) {
                    new_genome->mutateAddNode(pop->innovations, pop->curNodeId, pop->curInnovNum);
                    //  std::cout<<"mutate_add_node: "<<new_genome<<std::endl;
                    mut_struct_baby = true;
                } else if (randFloat() < NEAT::mutateAddLinkProb) {
                    net_analogue = new_genome->genesis(generation);
                    new_genome->mutateAddLink(pop->innovations, pop->curInnovNum, NEAT::newLinkTries);
                    delete net_analogue;
                    //std::cout<<"mutate_add_link: "<<new_genome<<std::endl;
                    mut_struct_baby = true;
                } else {
                    //Only do other mutations when not doing sturctural mutations

                    if (randFloat() < NEAT::mutateRandomTraitProb) {
                        new_genome->mutateRandomTrait();
                        //std::cout<<"..mutate random trait: "<<new_genome<<std::endl;
                    }
                    if (randFloat() < NEAT::mutateLinkTraitProb) {
                        new_genome->mutateLinkTrait(1);
                        //std::cout<<"..mutate link trait: "<<new_genome<<std::endl;
                    }
                    if (randFloat() < NEAT::mutateNodeTraitProb) {
                        new_genome->mutateNodeTrait(1);
                        //std::cout<<"mutate_node_trait: "<<new_genome<<std::endl;
                    }
                    if (randFloat() < NEAT::mutateLinkWeightsProb) {
                        new_genome->mutateLinkWeights(mut_power, 1.0, GAUSSIAN);
                        //std::cout<<"mutate_link_weights: "<<new_genome<<std::endl;
                    }
                    if (randFloat() < NEAT::mutateToggleEnableProb) {
                        new_genome->mutateToggleEnable(1);
                        //std::cout<<"mutate_toggle_enable: "<<new_genome<<std::endl;
                    }
                    if (randFloat() < NEAT::mutateGeneReEnableProb) {
                        new_genome->mutateGeneReEnable();
                        //std::cout<<"mutate_gene_reenable: "<<new_genome<<std::endl;
                    }
                }

                //Create the baby
                baby = new Organism(0.0, new_genome, generation);

            } else {
                //Create the baby without mutating first
                baby = new Organism(0.0, new_genome, generation);
            }

        }

        //Add the baby to its proper Species
        //If it doesn't fit a Species, create a new one

        baby->mutStructBaby = mut_struct_baby;
        baby->mateBaby = mate_baby;

        curspecies = (pop->species).begin();
        if (curspecies == (pop->species).end()) {
            //Create the first species
            newspecies = new Species(++(pop->lastSpecies), true);
            (pop->species).push_back(newspecies);
            newspecies->addOrganism(baby);  //Add the baby
            baby->species = newspecies;  //Point the baby to its species
        } else {
            comporg = (*curspecies)->first();
            found = false;
            while ((curspecies != (pop->species).end()) &&
                   (!found)) {
                if (comporg == 0) {
                    //Keep searching for a matching species
                    ++curspecies;
                    if (curspecies != (pop->species).end())
                        comporg = (*curspecies)->first();
                } else if (((baby->gnome)->compatibility(comporg->gnome)) < NEAT::compatThreshold) {
                    //Found compatible species, so add this organism to it
                    (*curspecies)->addOrganism(baby);
                    baby->species = (*curspecies);  //Point organism to its species
                    found = true;  //Note the search is over
                } else {
                    //Keep searching for a matching species
                    ++curspecies;
                    if (curspecies != (pop->species).end())
                        comporg = (*curspecies)->first();
                }
            }

            //If we didn't find a match, create a new species
            if (found == false) {
                newspecies = new Species(++(pop->lastSpecies), true);
                //std::std::cout<<"CREATING NEW SPECIES "<<pop->last_species<<std::std::endl;
                (pop->species).push_back(newspecies);
                newspecies->addOrganism(baby);  //Add the baby
                baby->species = newspecies;  //Point baby to its species
            }


        } //end else

    }


    return true;
}

bool NEAT::orderSpecies(Species *x, Species *y) {
    //std::cout<<"Comparing "<<((*((x->organisms).begin()))->orig_fitness)<<" and "<<((*((y->organisms).begin()))->orig_fitness)<<": "<<(((*((x->organisms).begin()))->orig_fitness) > ((*((y->organisms).begin()))->orig_fitness))<<std::endl;
    return (((*((x->organisms).begin()))->origFitness) > ((*((y->organisms).begin()))->origFitness));
}

bool NEAT::orderNewSpecies(Species *x, Species *y) {
    return (x->computeMaxFitness() > y->computeMaxFitness());
}


