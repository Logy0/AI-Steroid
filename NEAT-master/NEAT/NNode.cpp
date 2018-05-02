#include "NNode.h"
#include <iostream>
#include <sstream>

using namespace NEAT;

NNode::NNode(nodeType nType, int nodeid) {
    activeFlag = false;
    activeSum = 0;
    activation = 0;
    output = 0;
    last_activation = 0;
    last_activation2 = 0;
    type = nType; //NEURON or SENSOR type
    activation_count = 0; //Inactive upon creation
    nodeId = nodeid;
    fType = SIGMOID;
    nodeTrait = 0;
    gen_node_label = HIDDEN;
    dup = 0;
    analogue = 0;
    frozen = false;
    traitId = 1;
    override = false;
}

NNode::NNode(nodeType nType, int nodeid, nodePlace placement) {
    activeFlag = false;
    activeSum = 0;
    activation = 0;
    output = 0;
    last_activation = 0;
    last_activation2 = 0;
    type = nType; //NEURON or SENSOR type
    activation_count = 0; //Inactive upon creation
    nodeId = nodeid;
    fType = SIGMOID;
    nodeTrait = 0;
    gen_node_label = placement;
    dup = 0;
    analogue = 0;
    frozen = false;
    traitId = 1;
    override = false;
}

NNode::NNode(NNode *n, Trait *t) {
    activeFlag = false;
    activation = 0;
    output = 0;
    last_activation = 0;
    last_activation2 = 0;
    type = n->type; //NEURON or SENSOR type
    activation_count = 0; //Inactive upon creation
    nodeId = n->nodeId;
    fType = SIGMOID;
    nodeTrait = 0;
    gen_node_label = n->gen_node_label;
    dup = 0;
    analogue = 0;
    nodeTrait = t;
    frozen = false;
    if (t != 0)
        traitId = t->traitId;
    else traitId = 1;
    override = false;
}

NNode::NNode(const char *argLine, std::vector<Trait *> &traits) {
    int traitnum;
    std::vector<Trait *>::iterator curtrait;

    activeSum = 0;

    std::stringstream ss(argLine);
    int nodety, nodepl;
    ss >> nodeId >> traitnum >> nodety >> nodepl;
    type = (nodeType) nodety;
    gen_node_label = (nodePlace) nodepl;

    // Get the Sensor Identifier and Parameter String
    // mySensor = SensorRegistry::getSensor(id, param);
    frozen = false;  //TODO: Maybe change

    //Get a pointer to the trait this node points to
    if (traitnum == 0) nodeTrait = 0;
    else {
        curtrait = traits.begin();
        while (((*curtrait)->traitId) != traitnum)
            ++curtrait;
        nodeTrait = (*curtrait);
        traitId = nodeTrait->traitId;
    }

    override = false;
}

// This one might be incomplete
NNode::NNode(const NNode &nnode) {
    activeFlag = nnode.activeFlag;
    activeSum = nnode.activeSum;
    activation = nnode.activation;
    output = nnode.output;
    last_activation = nnode.last_activation;
    last_activation2 = nnode.last_activation2;
    type = nnode.type; //NEURON or SENSOR type
    activation_count = nnode.activation_count; //Inactive upon creation
    nodeId = nnode.nodeId;
    fType = nnode.fType;
    nodeTrait = nnode.nodeTrait;
    gen_node_label = nnode.gen_node_label;
    dup = nnode.dup;
    analogue = nnode.dup;
    frozen = nnode.frozen;
    traitId = nnode.traitId;
    override = nnode.override;
}

NNode::~NNode() {
    std::vector<Link *>::iterator curlink;

    //Kill off all incoming links
    for (curlink = incoming.begin(); curlink != incoming.end(); ++curlink) {
        delete (*curlink);
    }
}

//Returns the type of the node, NEURON or SENSOR
const nodeType NNode::getType() {
    return type;
}

//If the node is a SENSOR, returns true and loads the value
bool NNode::sensorLoad(double value) {
    if (type == SENSOR) {

        //Time delay memory
        last_activation2 = last_activation;
        last_activation = activation;

        activation_count++;  //Puts sensor into next time-step
        activation = value;
        return true;
    } else return false;
}

// Note: NEAT keeps track of which links are recurrent and which
// are not even though this is unnecessary for activation.
// It is useful to do so for 2 other reasons: 
// 1. It makes networks visualization of recurrent networks possible
// 2. It allows genetic control of the proportion of connections
//    that may become recurrent

// Return activation currently in node, if it has been activated
double NNode::getActiveOut() {
    if (activation_count > 0)
        return activation;
    else return 0.0;
}

// Return activation currently in node from PREVIOUS (time-delayed) time step,
// if there is one
double NNode::getActiveOutTd() {
    if (activation_count > 1)
        return last_activation;
    else return 0.0;
}

// This recursively flushes everything leading into and including this NNode, including recurrencies
void NNode::flushBack() {
    std::vector<Link *>::iterator curlink;

    //A sensor should not flush black
    if (type != SENSOR) {

        if (activation_count > 0) {
            activation_count = 0;
            activation = 0;
            last_activation = 0;
            last_activation2 = 0;
        }

        //Flush back recursively
        for (curlink = incoming.begin(); curlink != incoming.end(); ++curlink) {
            //Flush the link itself (For future learning parameters possibility)
            (*curlink)->added_weight = 0;
            if ((((*curlink)->inNode)->activation_count > 0))
                ((*curlink)->inNode)->flushBack();
        }
    } else {
        //Flush the SENSOR
        activation_count = 0;
        activation = 0;
        last_activation = 0;
        last_activation2 = 0;

    }

}

// This recursively checks everything leading into and including this NNode, 
// including recurrencies
// Useful for debugging
void NNode::flushBackCheck(std::vector<NNode *> &seenList) {
    std::vector<Link *>::iterator curlink;
    //int pause;
    std::vector<Link *> innodes = incoming;
    std::vector<NNode *>::iterator location;

    if (type != SENSOR) {


        //std::cout<<"ALERT: "<<this<<" has activation count "<<activation_count<<std::endl;
        //std::cout<<"ALERT: "<<this<<" has activation  "<<activation<<std::endl;
        //std::cout<<"ALERT: "<<this<<" has last_activation  "<<last_activation<<std::endl;
        //std::cout<<"ALERT: "<<this<<" has last_activation2  "<<last_activation2<<std::endl;

        if (activation_count > 0) {
            std::cout << "ALERT: " << this << " has activation count " << activation_count << std::endl;
        }

        if (activation > 0) {
            std::cout << "ALERT: " << this << " has activation  " << activation << std::endl;
        }

        if (last_activation > 0) {
            std::cout << "ALERT: " << this << " has last_activation  " << last_activation << std::endl;
        }

        if (last_activation2 > 0) {
            std::cout << "ALERT: " << this << " has last_activation2  " << last_activation2 << std::endl;
        }

        for (curlink = innodes.begin(); curlink != innodes.end(); ++curlink) {
            location = std::find(seenList.begin(), seenList.end(), ((*curlink)->inNode));
            if (location == seenList.end()) {
                seenList.push_back((*curlink)->inNode);
                ((*curlink)->inNode)->flushBackCheck(seenList);
            }
        }

    } else {
        //Flush_check the SENSOR
        std::cout << "sALERT: " << this << " has activation count " << activation_count << std::endl;
        std::cout << "sALERT: " << this << " has activation  " << activation << std::endl;
        std::cout << "sALERT: " << this << " has last_activation  " << last_activation << std::endl;
        std::cout << "sALERT: " << this << " has last_activation2  " << last_activation2 << std::endl;


        if (activation_count > 0) {
            std::cout << "ALERT: " << this << " has activation count " << activation_count << std::endl;
        }

        if (activation > 0) {
            std::cout << "ALERT: " << this << " has activation  " << activation << std::endl;
        }

        if (last_activation > 0) {
            std::cout << "ALERT: " << this << " has last_activation  " << last_activation << std::endl;
        }

        if (last_activation2 > 0) {
            std::cout << "ALERT: " << this << " has last_activation2  " << last_activation2 << std::endl;
        }

    }

}

// Reserved for future system expansion
void NNode::deriveTrait(Trait *curTrait) {

    if (curTrait != 0) {
        for (int count = 0; count < NEAT::numTraitParams; count++)
            params[count] = (curTrait->params)[count];
    } else {
        for (int count = 0; count < NEAT::numTraitParams; count++)
            params[count] = 0;
    }

    if (curTrait != 0)
        traitId = curTrait->traitId;
    else traitId = 1;

}

// Force an output value on the node
void NNode::overrideOutput(double newOutput) {
    override_value = newOutput;
    override = true;
}

// Tell whether node has been overridden
bool NNode::overridden() {
    return override;
}

// Set activation to the override value and turn off override
void NNode::activateOverride() {
    activation = override_value;
    override = false;
}


void NNode::printToFile(std::ofstream &outFile) {
    outFile << "node " << nodeId << " ";
    if (nodeTrait != 0) outFile << nodeTrait->traitId << " ";
    else outFile << "0 ";
    outFile << type << " ";
    outFile << gen_node_label << std::endl;
}


void NNode::printToFile(std::ostream &outFile) {
    char tempbuf[128];
    sprintf_s(tempbuf, "node %d ", nodeId);
    outFile << tempbuf;

    if (nodeTrait != 0) {
        char tempbuf2[128];
        sprintf_s(tempbuf2, "%d ", nodeTrait->traitId);
        outFile << tempbuf2;
    } else outFile << "0 ";

    char tempbuf2[128];
    sprintf_s(tempbuf2, "%d %d\n", type, gen_node_label);
    outFile << tempbuf2;
}

//Find the greatest depth starting from this neuron at depth d
int NNode::depth(int d, Network *mynet) {
    std::vector<Link *> innodes = this->incoming;
    std::vector<Link *>::iterator curlink;
    int cur_depth; //The depth of the current node
    int max = d; //The max depth

    if (d > 100) {
        return 10;
    }

    //Base Case
    if ((this->type) == SENSOR)
        return d;
        //Recursion
    else {

        for (curlink = innodes.begin(); curlink != innodes.end(); ++curlink) {
            cur_depth = ((*curlink)->inNode)->depth(d + 1, mynet);
            if (cur_depth > max) max = cur_depth;
        }

        return max;

    }

}
