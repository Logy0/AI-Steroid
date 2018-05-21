#ifndef _GENOME_H_
#define _GENOME_H_

#include <vector>
#include "Gene.h"
#include "Innovation.h"

namespace NEAT {

    enum mutator {
        GAUSSIAN = 0,
        COLDGAUSSIAN = 1
    };

    /**
    *	A Genome is the primary source of genotype information used to create
    *	a phenotype.  It contains 3 major constituents:
    *	  1) A list of Traits
    *	  2) A list of NNodes pointing to a Trait from (1)
    *	  3) A list of Genes with Links that point to Traits from (1)
    *	(1) Reserved parameter space for future use
    *	(2) NNode specifications
    *	(3) Is the primary source of innovation in the evolutionary Genome.
    *	    Each Gene in (3) has a marker telling when it arose historically.
    *	    Thus, these Genes can be used to speciate the population, and the
    *	    list of Genes provide an evolutionary history of innovation and
    *	    link-building.
    */
    class Genome {

    public:
        int genomeId;

        /**parameter conglomerations*/
        std::vector<Trait *> traits;
        /**List of NNodes for the Network*/
        std::vector<NNode *> nodes;
        /**List of innovation-tracking genes*/
        std::vector<Gene *> genes;

        /**Allows Genome to be matched with its Network*/
        Network *phenotype;

        /**Return id of final NNode in Genome*/
        int getLastNodeId();

        /**Return last innovation number in Genome*/
        double getLastGeneInnovationNum();

        /**Displays Genome on screen*/
        void printGenome();

        /**Constructor which takes full genome specs and puts them into the new one*/
        Genome(int id, std::vector<Trait *> t, std::vector<NNode *> n, std::vector<Gene *> g);

        /**Constructor which takes in links (not genes) and creates a Genome*/
        Genome(int id, std::vector<Trait *> t, std::vector<NNode *> n, std::vector<Link *> links);

        /** Copy constructor*/
        Genome(const Genome &genome);

        /**
         * Special constructor which spawns off an input file
         * This constructor assumes that some routine has already read in GENOMESTART*/
        Genome(int id, std::ifstream &iFile);

        /** This special constructor creates a Genome
         * with i inputs, o outputs, n out of nMax hidden units, and random
         * connectivity.  If r is true then recurrent connections will
         * be included.
         * The last input is a bias
         * LinkProb is the probability of a link*/
        Genome(int new_id, int i, int o, int n, int nMax, bool r, double linkProb);

        /** Special constructor that creates a Genome of 3 possible types:
         * 0 - Fully linked, no hidden nodes
         * 1 - Fully linked, one hidden node splitting each link
         * 2 - Fully connected with a hidden layer, recurrent
         * numHidden is only used in type 2*/
        Genome(int numIn, int numOut, int numHidden, int type);

        /** Loads a new Genome from a file (doesn't require knowledge of Genome's id)*/
        static Genome *newGenomeLoad(char *fileName);

        /**Destructor kills off all lists (including the trait vector)*/
        ~Genome();

        /**Generate a network phenotype from this Genome with specified id*/
        Network *genesis(int);

        /** Dump this genome to specified file*/
        void printToFile(std::ostream &outFile);

        void printToFile(std::ofstream &outFile);

        /** Wrapper for print_to_file above*/
        void printToFileName(char *fileName);

        /** Duplicate this Genome to create a new one with the specified id*/
        Genome *duplicate(int new_id);

        /** For debugging: A number of tests can be run on a genome to check its
         * integrity
         * Note: Some of these tests do not indicate a bug, but rather are meant
         * to be used to detect specific system states*/
        bool verifyGenome();

        /******** MUTATORS ********/

        /** Perturb params in one trait*/
        void mutateRandomTrait();

        /** Change random link's trait. Repeat times times*/
        void mutateLinkTrait(int times);

        /** Change random node's trait times times*/
        void mutateNodeTrait(int times);

        /** Add Gaussian noise to linkweights either GAUSSIAN or COLDGAUSSIAN (from zero)*/
        void mutateLinkWeights(double power, double rate, mutator mutType);

        /** toggle genes on or off*/
        void mutateToggleEnable(int times);

        /** Find first disabled gene and enable it*/
        void mutateGeneReEnable();

        /** These last kinds of mutations return false if they fail
         *  They can fail under certain conditions,  being unable
         *  to find a suitable place to make the mutation.
         * Generally, if they fail, they can be called again if desired.
        */
        /** Mutate genome by adding a node respresentation*/
        bool mutateAddNode(std::vector<Innovation *> &innovs, int &curnodeId, double &curInnov);

        /** Mutate the genome by adding a new link between 2 random NNodes*/
        bool mutateAddLink(std::vector<Innovation *> &innovs, double &curInnov, int tries);

        void mutateAddSensor(std::vector<Innovation *> &innovs, double &curInnov);

        /******* MATING METHODS ******/

        /**
         * This method mates this Genome with another Genome g.
         * For every point in each Genome, where each Genome shares
         * the innovation number, the Gene is chosen randomly from
         * either parent.  If one parent has an innovation absent in
         * the other, the baby will inherit the innovation
         * Interspecies mating leads to all genes being inherited.
         * Otherwise, excess genes come from most fit parent.*/
        Genome *mateMultiPoint(Genome *g, int genomeid, double fitness1, double fitness2, bool interspec_flag);

        /**
         * This method mates like multipoint but instead of selecting one
         * or the other when the innovation numbers match, it averages their
         * weights
         */
        Genome *mateMultiPointAvg(Genome *g, int genomeid, double fitness1, double fitness2, bool interspec_flag);

        /**
         * This method is similar to a standard single point CROSSOVER
         * operator.  Traits are averaged as in the previous 2 mating
         * methods.  A point is chosen in the smaller Genome for crossing
         * with the bigger one.
         */
        Genome *mateSinglePoint(Genome *g, int genomeid);


        /********* COMPATIBILITY CHECKING METHODS *********/
        /**
         * This function gives a measure of compatibility between
         *  two Genomes by computing a linear combination of 3
         *  characterizing variables of their compatibilty.
         *  The 3 variables represent PERCENT DISJOINT GENES,
         *  PERCENT EXCESS GENES, MUTATIONAL DIFFERENCE WITHIN
         *  MATCHING GENES.  So the formula for compatibility
         *  is:  disjoint_coeff*pdg+excess_coeff*peg+mutdiff_coeff*mdmg.
         *  The 3 coefficients are global system parameters
          */
        double compatibility(Genome *g);

        double traitCompare(Trait *t1, Trait *t2);

        /**Return number of non-disabled genes*/
        int extrons();

        /** Randomize the trait pointers of all the node and connection genes*/
        void randomize_traits();

    protected:
        /**Inserts a NNode into a given ordered list of NNodes in order*/
        void node_insert(std::vector<NNode *> &nlist, NNode *n);

        /**
         * Adds a new gene that has been created through a mutation in the
         * correct order* into the list of genes in the genome
        */
        void addGene(std::vector<Gene *> &glist, Gene *g);

    };

    /**
    * Calls special constructor that creates a Genome of 3 possible types:
    * 0 - Fully linked, no hidden nodes
    * 1 - Fully linked, one hidden node splitting each link
    * 2 - Fully connected with a hidden layer
    * num_hidden is only used in type 2
    * Saves to file "auto_genome"*/
    Genome *newGenomeAuto(int num_in, int num_out, int num_hidden, int type, const char *filename);

    void printGenomeToFile(Genome *g, const char *filename);

}

#endif
