#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <algorithm>
#include <vector>
#include "Neat.h"
#include "NNode.h"

namespace NEAT {

    class Genome;

    /**
     * A NETWORK is a LIST of input NODEs and a LIST of output NODEs
     * The point of the network is to define a single entity which can evolve
     * or learn on its own, even though it may be part of a larger framework
    */
    class Network {

        friend class Genome;
    public:

        /** The number of nodes in the net (-1 means not yet counted)*/
        int numNodes;
        /** The number of links in the net (-1 means not yet counted)*/
        int numLinks;

        /** A list of all the nodes*/
        std::vector<NNode *> allNodes;

        /** For GUILE network inputting*/
        std::vector<NNode *>::iterator inputIter;

        /** Kills all nodes and links within*/
        void destroy();
        /** helper for above*/
        void destroyHelper(NNode *curNode, std::vector<NNode *> &seenList);

        void nodeCountHelper(NNode *curNode, int &counter, std::vector<NNode *> &seenList);

        void linkCountHelper(NNode *curNode, int &counter, std::vector<NNode *> &seenList);

    public:

        /** Allows Network to be matched with its Genome*/
        Genome *genotype;

        /** Every Network or subNetwork can have a name*/
        char *name;
        /** NNodes that input into the network*/
        std::vector<NNode *> inputs;
        /** Values output by the network*/
        std::vector<NNode *> outputs;
        /** Allow for a network id*/
        int netId;
        /** Maximum weight in network for adaptation purposes*/
        double maxWeight;

        /** Tells whether network can adapt or not*/
        bool adaptable;

       /**
        * This constructor allows the input and output lists to be supplied
        * Defaults to not using adaptation*/
        Network(std::vector<NNode *> in, std::vector<NNode *> out, std::vector<NNode *> all, int netid);

        /** Same as previous constructor except the adaptibility can be set true or false with adaptval*/
        Network(std::vector<NNode *> in, std::vector<NNode *> out, std::vector<NNode *> all, int netid, bool adaptval);

        /** This constructs a net with empty input and output lists*/
        Network(int netid);

        /** Same as previous constructor except the adaptibility can be set true or false with adaptval*/
        Network(int netid, bool adaptval);

        /** Copy Constructor*/
        Network(const Network &network);

        ~Network();

        /** Activates the net such that all outputs are active*/
        bool activate();

        /** Takes an array of sensor values and loads it into SENSOR inputs ONLY*/
        void loadSensors(double *value);

        /**
         * This checks a POTENTIAL link between a potential in_node
         * and potential out_node to see if it must be recurrent
         * Use count and thresh to jump out in the case of an infinite loop*/
        bool isRecur(NNode *potin_node, NNode *potout_node, int &count, int thresh);

        /** If all output are not active then return true*/
        bool outputsOff();

    };

} // namespace NEAT

#endif
