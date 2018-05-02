#include "Link.h"

using namespace NEAT;

Link::Link(double weight, NNode *inNode, NNode *outNode, bool isRecurrent) {
    this->weight = weight;
    this->inNode = inNode;
    this->outNode = outNode;
    this->isRecurrent = isRecurrent;
    added_weight = 0;
    linkTrait = 0;
    timeDelay = false;
    traitId = 1;
}

Link::Link(Trait *linkTrait, double weight, NNode *inNode, NNode *outNode, bool isRecurrent) {
    this->weight = weight;
    this->inNode = inNode;
    this->outNode = outNode;
    this->isRecurrent = isRecurrent;
    added_weight = 0;
    this->linkTrait = linkTrait;
    timeDelay = false;
    if (linkTrait != 0)
        traitId = linkTrait->traitId;
    else traitId = 1;
}

Link::Link(double weight) {
    this->weight = weight;
    inNode = outNode = 0;
    isRecurrent = false;
    linkTrait = 0;
    timeDelay = false;
    traitId = 1;
}

Link::Link(const Link &link) {
    weight = link.weight;
    inNode = link.inNode;
    outNode = link.outNode;
    isRecurrent = link.isRecurrent;
    added_weight = link.added_weight;
    linkTrait = link.linkTrait;
    timeDelay = link.timeDelay;
    traitId = link.traitId;
}

void Link::deriveTrait(Trait *curTrait) {

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
