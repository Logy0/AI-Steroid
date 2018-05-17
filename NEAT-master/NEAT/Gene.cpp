#include "Gene.h"

#include <iostream>
#include <sstream>

using namespace NEAT;

Gene::Gene(double weight, NNode *inNode, NNode *outNode, bool isRecurrent, double innovationNum, double mutationNum) {
    link = new Link(weight, inNode, outNode, isRecurrent);
    this->innovationNum = innovationNum;
    this->mutationNum = mutationNum;

    enable = true;
    frozen = false;
}


Gene::Gene(Trait *tp, double weight, NNode *inNode, NNode *outNode, bool isRecurrent, double innovationNum,
           double mutationNum) {
    link = new Link(tp, weight, inNode, outNode, isRecurrent);
    this->innovationNum = innovationNum;
    this->mutationNum = mutationNum;

    enable = true;
    frozen = false;
}

Gene::Gene(Gene *g, Trait *tp, NNode *inNode, NNode *outNode) {
    link = new Link(tp, (g->link)->weight, inNode, outNode, (g->link)->isRecurrent);
    innovationNum = g->innovationNum;
    mutationNum = g->mutationNum;

    enable = g->enable;
    frozen = g->frozen;
}

Gene::Gene(const char *argLine, std::vector<Trait *> &traits, std::vector<NNode *> &nodes) {
    //Gene parameter holders
    int traitNum;
    int inNodeNum;
    int outNodeNum;
    NNode *inNode;
    NNode *outNode;
    double weight;
    int recur;
    Trait *traitPtr;

    std::vector<Trait *>::iterator curTrait;
    std::vector<NNode *>::iterator curNode;

    //Get the gene parameters
    std::stringstream ss(argLine);
    ss >> traitNum >> inNodeNum >> outNodeNum >> weight >> recur >> innovationNum >> mutationNum >> enable;

    frozen = false; //TODO: MAYBE CHANGE

    //Get a pointer to the linktrait
    if (traitNum == 0) traitPtr = 0;
    else {
        curTrait = traits.begin();
        while (((*curTrait)->traitId) != traitNum)
            ++curTrait;
        traitPtr = (*curTrait);
    }

    //Get a pointer to the input node
    curNode = nodes.begin();
    while (((*curNode)->nodeId) != inNodeNum)
        ++curNode;
    inNode = (*curNode);

    //Get a pointer to the output node
    curNode = nodes.begin();
    while (((*curNode)->nodeId) != outNodeNum)
        ++curNode;
    outNode = (*curNode);

    link = new Link(traitPtr, weight, inNode, outNode, (bool) recur);
}

Gene::Gene(const Gene &gene) {
    this->innovationNum = gene.innovationNum;
    this->mutationNum = gene.mutationNum;
    this->enable = gene.enable;
    this->frozen = gene.frozen;
    this->link = new Link(*gene.link);
}

Gene::~Gene() {
    delete this->link;
}


void Gene::printToFile(std::ofstream &outFile) {
    outFile << "gene ";
    //Start off with the trait number for this gene
    if ((this->link->linkTrait) == 0) outFile << "0 ";
    else outFile << ((this->link->linkTrait)->traitId) << " ";
    outFile << (this->link->inNode)->nodeId << " ";
    outFile << (this->link->outNode)->nodeId << " ";
    outFile << (this->link->weight) << " ";
    outFile << (this->link->isRecurrent) << " ";
    outFile << this->innovationNum << " ";
    outFile << this->mutationNum << " ";
    outFile << this->enable << std::endl;
}


void Gene::printToFile(std::ostream &outFile) {
    outFile << "gene ";
    //outFile.write(5, "gene ");

    //Start off with the trait number for this gene
    if ((link->linkTrait) == 0) {
        outFile << "0 ";
        //outFile.write(2, "0 ");
    } else {
        outFile << ((link->linkTrait)->traitId) << " ";
    }
    outFile << (link->inNode)->nodeId << " ";
    outFile << (link->outNode)->nodeId << " ";
    outFile << (link->weight) << " ";
    outFile << (link->isRecurrent) << " ";
    outFile << innovationNum << " ";
    outFile << mutationNum << " ";
    outFile << enable << std::endl;
}
