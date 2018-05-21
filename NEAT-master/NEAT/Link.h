#ifndef _LINK_H_
#define _LINK_H_

#include "Neat.h"
#include "Trait.h"
#include "NNode.h"

namespace NEAT {

    class NNode;

    /**
     * A LINK is a connection from one node to another with an associated weight
     * It can be marked as recurrent
     * Its parameters are made public for efficiency
     */
    class Link {
    public:
        /**@brief Weight of connection */
        double weight;
        /**@brief  NNode inputting into the link*/
        NNode *inNode;
        /**@brief NNode that the link affects*/
        NNode *outNode;

        bool isRecurrent;
        bool timeDelay;

        /**@brief Points to a trait of parameters for genetic creation*/
        Trait *linkTrait;
        /**@brief identify the trait derived by this link*/
        int traitId;

        /**
         * LEARNING PARAMETERS
         * These are link-related parameters that change during Hebbian type learning
        */

        /**@brief The amount of weight adjustment*/
        double added_weight;
        double params[NEAT::numTraitParams];

        Link(double weight, NNode *inNode, NNode *outNode, bool isRecurrent);

        /**@brief Including a trait pointer in the Link creation*/
        Link(Trait *linkTrait, double weight, NNode *inNode, NNode *outNode, bool isRecurrent);

        /**@brief For when you don't know the connections yet*/
        Link(double weight);

        /** Copy Constructor*/
        Link(const Link &link);

        /**@brief Derive a trait into link params*/
        void deriveTrait(Trait *curTrait);

    };

}

#endif
