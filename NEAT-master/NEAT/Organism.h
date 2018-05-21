#ifndef _ORGANISM_H_
#define _ORGANISM_H_

#include "Genome.h"
#include "Species.h"

namespace NEAT {

    class Species;

    class Population;

    /**
     * ORGANISM CLASS:Organisms are Genomes and Networks with fitness
     * information
     * i.e. The genotype and phenotype together
     * */
    class Organism {

    public:
        /** A measure of fitness for the Organism*/
        double fitness;
        /** A fitness measure that won't change during adjustments*/
        double origFitness;
        /** Used just for reporting purposes*/
        double error;
        /** Win marker (if needed for a particular task)*/
        bool winner;
        /** The Organism's phenotype*/
        Network *net;
        /** The Organism's genotype */
        Genome *gnome;
        /** The Organism's Species */
        Species *species;
        /** Number of children this Organism may have*/
        double expectedOffspring;
        /** Tells which generation this Organism is from*/
        int generation;
        /** Marker for destruction of inferior Organisms*/
        bool eliminate;
        /** Marks the species champ*/
        bool champion;
        /** Number of reserved offspring for a population leader*/
        int superChampOffspring;
        /** Marks the best in population*/
        bool popChamp;
        /** Marks the duplicate child of a champion (for tracking purposes)*/
        bool popChampChild;
        /** DEBUG variable- high fitness of champ*/
        double highFit;
        /** When playing in real-time allows knowing the maturity of an individual*/
        int timeAlive;

        /** Track its origin- for debugging or analysis- we can tell how the organism was born*/
        bool mutStructBaby;
        bool mateBaby;

        /** MetaData for the object*/
        char metadata[128];
        bool modified;

        /** Print the Organism's genome to a file preceded by a comment detailing the organism's species, number, and fitness*/
        bool printToFile(char *filename);

        bool writeToFile(std::ostream &outFile);

        Organism(double fit, Genome *g, int gen, const char *md = 0);

        /** Copy Constructor*/
        Organism(const Organism &org);

        ~Organism();

    };

    /** This is used for list sorting of Organisms by fitness..highest fitness first*/
    bool order_orgs(Organism *x, Organism *y);

} // namespace NEAT

#endif
