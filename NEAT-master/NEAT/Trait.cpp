#include "Trait.h"

#include <iostream>
#include <sstream>

using namespace NEAT;

Trait::Trait() {
    for (int count = 0; count < NEAT::numTraitParams; count++)
        params[count] = 0;
    traitId = 0;
}

Trait::Trait(int id, double p1, double p2, double p3, double p4, double p5, double p6, double p7) {
    traitId = id;
    params[0] = p1;
    params[1] = p2;
    params[2] = p3;
    params[3] = p4;
    params[4] = p5;
    params[5] = p6;
    params[6] = p7;
    params[7] = 0;
}

Trait::Trait(const Trait &t) {
    for (int count = 0; count < NEAT::numTraitParams; count++)
        params[count] = (t.params)[count];

    traitId = t.traitId;
}


Trait::Trait(Trait *t) {
    for (int count = 0; count < NEAT::numTraitParams; count++)
        params[count] = (t->params)[count];

    traitId = t->traitId;
}


Trait::Trait(const char *argLine) {

    std::stringstream ss(argLine);
    ss >> traitId;
    //IS THE STOPPING CONDITION CORRECT?  ALERT
    for (int count = 0; count < NEAT::numTraitParams; count++) {
        ss >> params[count];
    }
}

Trait::Trait(Trait *t1, Trait *t2) {
    for (int count = 0; count < NEAT::numTraitParams; count++)
        params[count] = (((t1->params)[count]) + ((t2->params)[count])) / 2.0;
    traitId = t1->traitId;
}

void Trait::printToFile(std::ofstream &outFile) {
    outFile << "trait " << traitId << " ";
    for (int count = 0; count < NEAT::numTraitParams; count++)
        outFile << params[count] << " ";

    outFile << std::endl;

}

void Trait::printToFile(std::ostream &outFile) {
    char tempBuf[128];
    sprintf_s(tempBuf, "trait %d ", traitId);
    outFile << tempBuf;

    for (int count = 0; count < NEAT::numTraitParams; count++) {
        char tempBuf2[128];
		sprintf_s(tempBuf2, "%f ", params[count]);
        outFile << tempBuf2;
    }
    outFile << std::endl;
}

void Trait::mutate() {
    for (int count = 0; count < NEAT::numTraitParams; count++) {
        if (randFloat() > NEAT::traitParamMutProb) {
            params[count] += (randPosNeg() * randFloat()) * NEAT::traitMutationPower;
            if (params[count] < 0) params[count] = 0;
            if (params[count] > 1.0) params[count] = 1.0;
        }
    }
}
