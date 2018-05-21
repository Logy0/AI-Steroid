#ifndef _NERO_NEAT_H_
#define _NERO_NEAT_H_

#include <cstdlib>
#include <cstring>

namespace NEAT {

    /** Minimum time alive to be considered for selection or death in real-time evolution*/
    extern int timeAliveMinimum;
    const int numTraitParams = 8;

    extern double traitParamMutProb;
    /** Power of mutation on a signle trait param*/
    extern double traitMutationPower;
    /** Amount that mutation_num changes for a trait change inside a link*/
    extern double linkTraitMutSig;
    /** Amount a mutation_num changes on a link connecting a node that changed its trait*/
    extern double nodeTraitMutSig;
    /** The power of a linkweight mutation*/
    extern double weightMutPower;
    /** Prob. that a link mutation which doesn't have to be recurrent will be made recurrent*/
    extern double recurProb;

    /**
     * These 3 global coefficients are used to determine the formula for
     * computating the compatibility between 2 genomes.  The formula is:
     * disjointCoeff*pdg+excessCoeff*peg+mutDiffCoeff*mdmg.
     * See the compatibility method in the Genome class for more info
     * They can be thought of as the importance of disjoint Genes,
     * excess Genes, and parametric difference between Genes of the
     * same function, respectively.*/
    extern double disjointCoeff;
    extern double excessCoeff;
    extern double mutDiffCoeff;

    /** This global tells compatibility threshold under which two Genomes are considered the same species*/
    extern double compatThreshold;

    /** Globals involved in the epoch cycle - mating, reproduction, etc..*/
    /** How much does age matter?*/
    extern double ageSignificance;
    /** Percent of ave fitness for survival*/
    extern double survivalThresh;
    /** Prob. of a non-mating reproduction*/
    extern double mutateOnlyProb;
    extern double mutateRandomTraitProb;
    extern double mutateLinkTraitProb;
    extern double mutateNodeTraitProb;
    extern double mutateLinkWeightsProb;
    extern double mutateToggleEnableProb;
    extern double mutateGeneReEnableProb;
    extern double mutateAddNodeProb;
    extern double mutateAddLinkProb;
    /** Prob. of a mate being outside species*/
    extern double interSpeciesMateRate;
    extern double mateMultiPointProb;
    extern double mateMultiPointAvgProb;
    extern double mateSinglePointProb;
    /** Prob. of mating without mutation*/
    extern double mateOnlyProb;
    /** Probability of forcing selection of ONLY links that are naturally recurrent*/
    extern double recurOnlyProb;
    /** Size of population*/
    extern int popSize;
    /** Age where Species starts to be penalized*/
    extern int dropOffAge;
    /** Number of tries mutate_add_link will attempt to find an open link*/
    extern int newLinkTries;
    /** Tells to print population to file every n generations*/
    extern int printEvery;
    /** The number of babies to siphon off to the champions*/
    extern int babiesStolen;
    /**number of times to run experiment*/
    extern int numRuns;

    int getUnitCount(const char *string, const char *set);

    /** Inline Random Functions*/
    extern inline int randPosNeg() {
        if (rand() % 2)
            return 1;
        else
            return -1;
    }

    extern inline int randInt(int x, int y) {
        return rand() % (y - x + 1) + x;
    }

    extern inline double randFloat() {
        return rand() / (double) RAND_MAX;
    }

    /** SIGMOID FUNCTION ********************************
     * This is a signmoidal activation function, which is an S-shaped squashing function
     * It smoothly limits the amplitude of the output of a neuron to between 0 and 1
     * It is a helper to the neural-activation function get_active_out
     * It is made inline so it can execute quickly since it is at every non-sensor
     * node in a network.
     * NOTE:  In order to make node insertion in the middle of a link possible,
     * the signmoid can be shifted to the right and more steeply sloped:
     * slope=4.924273
     * constant= 2.4621365
     * These parameters optimize mean squared error between the old output,
     * and an output of a node inserted in the middle of a link between
     * the old output and some other node.
     * When not right-shifted, the steepened slope is closest to a linear
     * ascent as possible between -0.5 and 0.5*/
    extern double sigmoidFunction(double activesum, double slope, double constant);

    /** Hebbian Adaptation Function
     * Based on equations in Floreano & Urzelai 2000
     * Takes the current weight, the maximum weight in the containing network,
     * the activation coming in and out of the synapse,
     * and three learning rates for hebbianFunction, presynaptic, and postsynaptic
     * modification
     * Returns the new modified weight
     * NOTE: For an inhibatory connection, it makes sense to
     *    emphasize decorrelation on hebbianFunction learning!*/
    extern double hebbianFunction(double weight, double maxweight, double active_in, double active_out,
                                  double hebb_rate, double pre_rate,
                                  double post_rate);

    /**Returns a normally distributed deviate with 0 mean and unit variance
     * Algorithm is from Numerical Recipes in C, Second Edition*/
    extern double gaussRand();

    //This is an incorrect gassian distribution...but it is faster than gaussRand (maybe it's good enough?)
    //inline double gaussrand_wrong() {return (randPosNeg())*(sqrt(-log((rand()*1.0)/RAND_MAX)));}

    bool loadNeatParams(const char *filename, bool output = false);

} // namespace NEAT

#endif
