#include "Organism.h"

using namespace NEAT;

Organism::Organism(double fit, Genome *g, int gen, const char *md) {
    fitness = fit;
    origFitness = fitness;
    gnome = g;
    net = gnome->genesis(gnome->genomeId);
    species = 0;  //Start it in no Species
    expectedOffspring = 0;
    generation = gen;
    eliminate = false;
    error = 0;
    winner = false;
    champion = false;
    superChampOffspring = 0;

    // If md is null, then we don't have metadata, otherwise we do have metadata so copy it over
    if (md == 0) {
        strcpy_s(metadata, "");
    } else {
        strncpy_s(metadata, md, 128);
    }

    timeAlive = 0;

    //DEBUG vars
    popChamp = false;
    popChampChild = false;
    highFit = 0;
    mutStructBaby = 0;
    mateBaby = 0;

    modified = true;
}

Organism::Organism(const Organism &org) {
    fitness = org.fitness;
    origFitness = org.origFitness;
    gnome = new Genome(*(org.gnome));    // Associative relationship
    //gnome = org.gnome->duplicate(org.gnome->genome_id);
    net = new Network(*(org.net)); // Associative relationship
    species = org.species;    // Delegation relationship
    expectedOffspring = org.expectedOffspring;
    generation = org.generation;
    eliminate = org.eliminate;
    error = org.error;
    winner = org.winner;
    champion = org.champion;
    superChampOffspring = org.superChampOffspring;

    strcpy_s(metadata, org.metadata);
    //printf("copying %s did it work? %s", org.metadata, metadata);

    timeAlive = org.timeAlive;
    popChamp = org.popChamp;
    popChampChild = org.popChampChild;
    highFit = org.highFit;
    mutStructBaby = org.mutStructBaby;
    mateBaby = org.mateBaby;

    modified = false;
}

Organism::~Organism() {
    delete net;
    delete gnome;
}

bool Organism::printToFile(char *filename) {

    std::ofstream oFile(filename);

    return writeToFile(oFile);
}

bool Organism::writeToFile(std::ostream &outFile) {

    char tempbuf2[1024];
    if (modified) {
        sprintf_s(tempbuf2, "/* Organism #%d Fitness: %f Time: %d */\n", (gnome)->genomeId, fitness, timeAlive);
    } else {
        sprintf_s(tempbuf2, "/* %s */\n", metadata);
    }
    outFile << tempbuf2;
    gnome->printToFile(outFile);
    return 1;
}

bool NEAT::order_orgs(Organism *x, Organism *y) {
    return (x)->fitness > (y)->fitness;
}

