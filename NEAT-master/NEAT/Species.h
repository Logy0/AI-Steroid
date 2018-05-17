#ifndef _SPECIES_H_
#define _SPECIES_H_

#include "Neat.h"
#include "Organism.h"
#include "Population.h"

namespace NEAT {

    class Organism;

    class Population;

    /**
     * SPECIES CLASS:A Species is a group of similar Organisms
     * Reproduction takes place mostly within a
     * single species, so that compatible organisms can mate.
     * */

    class Species {

    public:

        int id;
        /**The age of the Species*/
        int age;
        /**The average fitness of the Species*/
        double avgFitness;
        /**Max fitness of the Species*/
        double maxFitness;
        /**The max it ever had*/
        double maxFitnessEver;
        int expectedOffspring;
        bool novel;
        bool checked;
        /**Allows killing off in competitive coevolution stagnation*/
        bool obliterate;
        /**The organisms in the Species*/
        std::vector<Organism *> organisms;
        /**If this is too long ago, the Species will goes extinct*/
        int ageOfLastImprovement;
        /**When playing real-time allows estimating average fitness*/
        double avgEst;

        bool addOrganism(Organism *o);

        Organism *first();

        bool printToFile(std::ostream &outFile);

        bool printToFile(std::ofstream &outFile);

        double computeMaxFitness();

        /**Remove an organism from Species*/
        bool removeOrg(Organism *org);

        double size() {
            return organisms.size();
        }

        Organism *get_champ();

        /**Perform mating and mutation to form next generation*/
        bool reproduce(int generation, Population *pop, std::vector<Species *> &sorted_species);

        /**** Real-time methods ****/

        /**Place organisms in this species in order by their fitness*/
        bool rank();

        /**Compute an estimate of the average fitness of the species
         * The result is left in variable average_est and returned
         * New variable: average_est, NEAT::time_alive_minimum (const)
         * Note: Initialization requires calling estimate_average() on all species
         *  Later it should be called only when a species changes*/
        double estimateAverage();

        /** Like the usual reproduce() method except only one offspring is produced
          * Note that "generation" will be used to just count which offspring # this is over all evolution
          * Here is how to get sorted species:
          *  Sort the Species by max fitness (Use an extra list to do this)
          *   These need to use ORIGINAL fitness
          *  sorted_species.sort(order_species);*/
        Organism *reproduceOne(int generation, Population *pop, std::vector<Species *> &sorted_species);

        Species(int i);

        /**
          * Allows the creation of a Species that won't age (a novel one)
          * This protects new Species from aging inside their first generation
        */
        Species(int i, bool n);

        ~Species();

    };

    /** This is used for list sorting of Species by fitness of best organism highest fitness first*/
    bool orderSpecies(Species *x, Species *y);

    bool orderNewSpecies(Species *x, Species *y);

}

#endif
