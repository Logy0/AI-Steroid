#include "Innovation.h"

using namespace NEAT;

Innovation::Innovation(int nin, int nout, double num1, double num2, int newid, double oldinnov) {
    innovationType = NEWNODE;
    nodeInId = nin;
    nodeOutId = nout;
    innovationNum1 = num1;
    innovationNum2 = num2;
    newNodeId = newid;
    oldInnovNum = oldinnov;

    //Unused parameters set to zero
    newWeight = 0;
    newTraitNum = 0;
    recurFlag = false;
}

Innovation::Innovation(int nin, int nout, double num1, double w, int t) {
    innovationType = NEWLINK;
    nodeInId = nin;
    nodeOutId = nout;
    innovationNum1 = num1;
    newWeight = w;
    newTraitNum = t;

    //Unused parameters set to zero
    innovationNum2 = 0;
    newNodeId = 0;
    recurFlag = false;
}

Innovation::Innovation(int nin, int nout, double num1, double w, int t, bool recur) {
    innovationType = NEWLINK;
    nodeInId = nin;
    nodeOutId = nout;
    innovationNum1 = num1;
    newWeight = w;
    newTraitNum = t;

    //Unused parameters set to zero
    innovationNum2 = 0;
    newNodeId = 0;
    recurFlag = recur;
}
