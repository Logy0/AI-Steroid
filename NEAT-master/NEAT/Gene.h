#ifndef _GENE_H_
#define _GENE_H_

#include "Neat.h"
#include "Trait.h"
#include "Link.h"
#include "Network.h"

namespace NEAT {

    class Gene {
    public:

        Link *link;
        double innovationNum;
        /**Used to see how much mutation has changed the link*/
        double mutationNum;
        /**When this is off the Gene is disabled*/
        bool enable;
        /**When frozen, the linkweight cannot be mutated*/
        bool frozen;

        /**Construct a gene with no trait*/
        Gene(double weight, NNode *inNode, NNode *outNode, bool isRecurrent, double innovationNum, double mutationNum);

        /**Construct a gene with a trait*/
        Gene(Trait *tp, double weight, NNode *inNode, NNode *outNode, bool isRecurrent, double innovationNum,
             double mutationNum);

        /**Construct a gene off of another gene as a duplicate*/
        Gene(Gene *g, Trait *tp, NNode *inNode, NNode *outNode);

        /**Construct a gene from a file spec given traits and nodes*/
        Gene(const char *argLine, std::vector<Trait *> &traits, std::vector<NNode *> &nodes);

        /** Copy Constructor*/
        Gene(const Gene &gene);

        ~Gene();

        /**Print gene to a file- called from Genome*/
        void printToFile(std::ostream &outFile);

        void printToFile(std::ofstream &outFile);
    };

}


#endif
