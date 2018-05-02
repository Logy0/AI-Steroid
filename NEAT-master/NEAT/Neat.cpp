#include "Neat.h"

#include <fstream>
#include <cmath>

int NEAT::timeAliveMinimum = 0;
double NEAT::traitParamMutProb = 0;
double NEAT::traitMutationPower = 0;    //// Power of mutation on a signle trait param
double NEAT::linkTraitMutSig = 0;       //// Amount that mutation_num changes for a trait change inside a link
double NEAT::nodeTraitMutSig = 0;       //// Amount a mutation_num changes on a link connecting a node that changed its trait
double NEAT::weightMutPower = 0;        //// The power of a linkweight mutation
double NEAT::recurProb = 0;             //// Prob. that a link mutation which doesn't have to be recurrent will be made recurrent
double NEAT::disjointCoeff = 0;
double NEAT::excessCoeff = 0;
double NEAT::mutDiffCoeff = 0;
double NEAT::compatThreshold = 0;
double NEAT::ageSignificance = 0;       //// How much does age matter?
double NEAT::survivalThresh = 0;        //// Percent of ave fitness for survival
double NEAT::mutateOnlyProb = 0;        //// Prob. of a non-mating reproduction
double NEAT::mutateRandomTraitProb = 0;
double NEAT::mutateLinkTraitProb = 0;
double NEAT::mutateNodeTraitProb = 0;
double NEAT::mutateLinkWeightsProb = 0;
double NEAT::mutateToggleEnableProb = 0;
double NEAT::mutateGeneReEnableProb = 0;
double NEAT::mutateAddNodeProb = 0;
double NEAT::mutateAddLinkProb = 0;
double NEAT::interSpeciesMateRate = 0;  //// Prob. of a mate being outside species
double NEAT::mateMultiPointProb = 0;
double NEAT::mateMultiPointAvgProb = 0;
double NEAT::mateSinglePointProb = 0;
double NEAT::mateOnlyProb = 0;          //// Prob. of mating without mutation
double NEAT::recurOnlyProb = 0;         //// Probability of forcing selection of ONLY links that are naturally recurrent
int NEAT::popSize = 0;                  //// Size of population
int NEAT::dropOffAge = 0;               //// Age where Species starts to be penalized
int NEAT::newLinkTries = 0;             //// Number of tries mutate_add_link will attempt to find an open link
int NEAT::printEvery = 0;               //// Tells to print population to file every n generations
int NEAT::babiesStolen = 0;             //// The number of babies to siphen off to the champions
int NEAT::numRuns = 0;

int NEAT::getUnitCount(const char *string, const char *set) {
    int count = 0;
    short last = 0;
    while (*string) {
        last = *string++;

        for (int i = 0; set[i]; i++) {
            if (last == set[i]) {
                count++;
                last = 0;
                break;
            }
        }
    }
    if (last)
        count++;
    return count;
}

bool NEAT::loadNeatParams(const char *filename, bool output) {

    std::ifstream paramFile(filename);

    if (!paramFile.is_open()) {
        return false;
    }
    char curword[128];

    // **********LOAD IN PARAMETERS*************** //
    if (output)
        printf("NEAT READING IN %s", filename);

    paramFile >> curword;
    paramFile >> NEAT::traitParamMutProb;

    paramFile >> curword;
    paramFile >> NEAT::traitMutationPower;

    paramFile >> curword;
    paramFile >> NEAT::linkTraitMutSig;

    paramFile >> curword;
    paramFile >> NEAT::nodeTraitMutSig;

    paramFile >> curword;
    paramFile >> NEAT::weightMutPower;

    paramFile >> curword;
    paramFile >> NEAT::recurProb;

    paramFile >> curword;
    paramFile >> NEAT::disjointCoeff;

    paramFile >> curword;
    paramFile >> NEAT::excessCoeff;

    paramFile >> curword;
    paramFile >> NEAT::mutDiffCoeff;

    paramFile >> curword;
    paramFile >> NEAT::compatThreshold;

    paramFile >> curword;
    paramFile >> NEAT::ageSignificance;

    paramFile >> curword;
    paramFile >> NEAT::survivalThresh;

    paramFile >> curword;
    paramFile >> NEAT::mutateOnlyProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateRandomTraitProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateLinkTraitProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateNodeTraitProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateLinkWeightsProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateToggleEnableProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateGeneReEnableProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateAddNodeProb;

    paramFile >> curword;
    paramFile >> NEAT::mutateAddLinkProb;

    paramFile >> curword;
    paramFile >> NEAT::interSpeciesMateRate;

    paramFile >> curword;
    paramFile >> NEAT::mateMultiPointProb;

    paramFile >> curword;
    paramFile >> NEAT::mateMultiPointAvgProb;

    paramFile >> curword;
    paramFile >> NEAT::mateSinglePointProb;

    paramFile >> curword;
    paramFile >> NEAT::mateOnlyProb;

    paramFile >> curword;
    paramFile >> NEAT::recurOnlyProb;

    paramFile >> curword;
    paramFile >> NEAT::popSize;

    paramFile >> curword;
    paramFile >> NEAT::dropOffAge;

    paramFile >> curword;
    paramFile >> NEAT::newLinkTries;

    paramFile >> curword;
    paramFile >> NEAT::printEvery;

    paramFile >> curword;
    paramFile >> NEAT::babiesStolen;

    paramFile >> curword;
    paramFile >> NEAT::numRuns;

    if (output) {
        printf("traitParamMutProb=%f\n", traitParamMutProb);
        printf("traitMutationPower=%f\n", traitMutationPower);
        printf("linkTraitMutSig=%f\n", linkTraitMutSig);
        printf("nodeTraitMutSig=%f\n", nodeTraitMutSig);
        printf("weightMutPower=%f\n", weightMutPower);
        printf("recurProb=%f\n", recurProb);
        printf("disjointCoeff=%f\n", disjointCoeff);
        printf("excessCoeff=%f\n", excessCoeff);
        printf("mutDiffCoeff=%f\n", mutDiffCoeff);
        printf("compatThreshold=%f\n", compatThreshold);
        printf("ageSignificance=%f\n", ageSignificance);
        printf("survivalThresh=%f\n", survivalThresh);
        printf("mutateOnlyProb=%f\n", mutateOnlyProb);
        printf("mutateRandomTraitProb=%f\n", mutateRandomTraitProb);
        printf("mutateLinkTraitProb=%f\n", mutateLinkTraitProb);
        printf("mutateNodeTraitProb=%f\n", mutateNodeTraitProb);
        printf("mutateLinkWeightsProb=%f\n", mutateLinkWeightsProb);
        printf("mutateToggleEnableProb=%f\n", mutateToggleEnableProb);
        printf("mutateGeneReEnableProb=%f\n", mutateGeneReEnableProb);
        printf("mutateAddNodeProb=%f\n", mutateAddNodeProb);
        printf("mutateAddLinkProb=%f\n", mutateAddLinkProb);
        printf("interSpeciesMateRate=%f\n", interSpeciesMateRate);
        printf("mateMultiPointProb=%f\n", mateMultiPointProb);
        printf("mateMultiPointAvgProb=%f\n", mateMultiPointAvgProb);
        printf("mateSinglePointProb=%f\n", mateSinglePointProb);
        printf("mateOnlyProb=%f\n", mateOnlyProb);
        printf("recurOnlyProb=%f\n", recurOnlyProb);
        printf("popSize=%d\n", popSize);
        printf("dropOffAge=%d\n", dropOffAge);
        printf("newLinkTries=%d\n", newLinkTries);
        printf("printEvery=%d\n", printEvery);
        printf("babiesStolen=%d\n", babiesStolen);
        printf("numRuns=%d\n", numRuns);
    }

    paramFile.close();
    return true;
}

double NEAT::gaussRand() {
    static int iset = 0;
    static double gset;
    double fac, rsq, v1, v2;

    if (iset == 0) {
        do {
            v1 = 2.0 * (randFloat()) - 1.0;
            v2 = 2.0 * (randFloat()) - 1.0;
            rsq = v1 * v1 + v2 * v2;
        } while (rsq >= 1.0 || rsq == 0.0);
        fac = sqrt(-2.0 * log(rsq) / rsq);
        gset = v1 * fac;
        iset = 1;
        return v2 * fac;
    } else {
        iset = 0;
        return gset;
    }
}

double NEAT::sigmoidFunction(double activesum, double slope, double constant) {
    return (1 / (1 + (exp(-(slope * activesum)))));
}

double NEAT::hebbianFunction(double weight, double maxweight, double active_in, double active_out, double hebb_rate,
                             double pre_rate,
                             double post_rate) {

    bool neg = false;
    double delta;

    double topweight;

    if (maxweight < 5.0) maxweight = 5.0;

    if (weight > maxweight) weight = maxweight;

    if (weight < -maxweight) weight = -maxweight;

    if (weight < 0) {
        neg = true;
        weight = -weight;
    }

    topweight = weight + 2.0;
    if (topweight > maxweight) topweight = maxweight;

    if (!(neg)) {
        delta =
                hebb_rate * (maxweight - weight) * active_in * active_out +
                pre_rate * (topweight) * active_in * (active_out - 1.0);
        return weight + delta;

    } else {
        //In the inhibatory case, we strengthen the synapse when output is low and
        //input is high
        delta =
                pre_rate * (maxweight - weight) * active_in * (1.0 - active_out) + //"unhebb"
                -hebb_rate * (topweight + 2.0) * active_in * active_out + //anti-hebbianFunction
                0;
        return -(weight + delta);

    }
}