#ifndef _NNODE_H_
#define _NNODE_H_

#include <algorithm>
#include <vector>
#include "Neat.h"
#include "Trait.h"
#include "Link.h"

namespace NEAT {

    enum nodeType {
        NEURON = 0,
        SENSOR = 1
    };

    enum nodePlace {
        HIDDEN = 0,
        INPUT = 1,
        OUTPUT = 2,
        BIAS = 3
    };

    enum funcType {
        SIGMOID = 0
    };

    class Link;

    class Network;

    /**
    * A NODE is either a NEURON or a SENSOR.
    *   - If it's a sensor, it can be loaded with a value for output
    *   - If it's a neuron, it has a list of its incoming input signals (List<Link> is used)
    * Use an activation count to avoid flushing
     * */
    class NNode {

        friend class Network;

        friend class Genome;

    protected:
        /** keeps track of which activation the node is currently in*/
        int activation_count;
        /** Holds the previous step's activation for recurrency*/
        double last_activation;
        /** Holds the activation BEFORE the prevous step's*/
        double last_activation2;
        /**
         * This is necessary for a special recurrent case when the innode
         * of a recurrent link is one time step ahead of the outnode.
         * The innode then needs to send from TWO time steps ago
         */

        /** Points to a trait of parameters*/
        Trait *nodeTrait;

        /** identify the trait derived by this node*/
        int traitId;

        /** Used for Genome duplication*/
        NNode *dup;

        /** Used for Gene decoding*/
        NNode *analogue;

        /** The NNode cannot compute its own output- something is overriding it*/
        bool override;

        /** Contains the activation value that will override this node's activation*/
        double override_value;

        /** Pointer to the Sensor corresponding to this Body.*/
        //Sensor* mySensor;

    public:
        /** When frozen, cannot be mutated (meaning its trait pointer is fixed)*/
        bool frozen;

        /** type is either SIGMOID ..or others that can be added*/
        funcType fType;
        /** type is either NEURON or SENSOR*/
        nodeType type;

        /** The incoming activity before being processed*/
        double activeSum;
        /** The total activation entering the NNode*/
        double activation;
        /** To make sure outputs are active*/
        bool activeFlag;

        /* NOT USED IN NEAT - covered by "activation" above*/
        /** Output of the NNode- the value in the NNode*/
        double output;

        /************* LEARNING PARAMETERS ************/
        /**
         * The following parameters are for use in
         * neurons that learn through habituation,
         * sensitization, or Hebbian-type processes*/

        double params[NEAT::numTraitParams];

        /** A list of pointers to incoming weighted signals from other nodes*/
        std::vector<Link *> incoming;
        /** A list of pointers to links carrying this node's signal*/
        std::vector<Link *> outgoing;

        /** These members are used for graphing with GTK+/GDK*/
        std::vector<double> rowlevels;  // Depths from output where this node appears
        int row;  // Final row decided upon for drawing this NNode in
        int ypos;
        int xpos;

        /** A node can be given an identification number for saving in files*/
        int nodeId;

        /** Used for genetic marking of nodes*/
        nodePlace gen_node_label;

        NNode(nodeType nType, int nodeid);

        NNode(nodeType nType, int nodeid, nodePlace placement);

        /** Construct a NNode off another NNode for genome purposes*/
        NNode(NNode *n, Trait *t);

        /** Construct the node out of a file specification using given list of traits*/
        NNode(const char *argLine, std::vector<Trait *> &traits);

        /** Copy Constructor*/
        NNode(const NNode &nnode);

        ~NNode();

        /** Just return activation for step*/
        double getActiveOut();

        /** Return activation from PREVIOUS time step*/
        double getActiveOutTd();

        /** Returns the type of the node, NEURON or SENSOR*/
        const nodeType getType();

        /** If the node is a SENSOR, returns true and loads the value*/
        bool sensorLoad(double value);

        /** Recursively deactivate backwards through the network*/
        void flushBack();

        /** Verify flushing for debugging*/
        void flushBackCheck(std::vector<NNode *> &seenList);

        /** Print the node to a file*/
        void printToFile(std::ostream &outFile);

        void printToFile(std::ofstream &outFile);

        /** Have NNode gain its properties from the trait*/
        void deriveTrait(Trait *curTrait);

        /** Force an output value on the node*/
        void overrideOutput(double newOutput);

        /** Tell whether node has been overridden*/
        bool overridden();

        /** Set activation to the override value and turn off override*/
        void activateOverride();

        /** Writes back changes weight values into the genome*/
        // (Lamarckian trasnfer of characteristics)
        void Lamarck();

        /**Find the greatest depth starting from this neuron at depth d*/
        int depth(int d, Network *mynet);

    };


} // namespace NEAT

#endif
