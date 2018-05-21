//
// Created by magzhan on 1/9/17.
//

#ifndef RTNEAT_CARTPOLE_H
#define RTNEAT_CARTPOLE_H

#include <cmath>
#include "Network.h"
#include <iostream>

using namespace std;
using namespace NEAT;

class CartPole {
public:
    CartPole(bool velocity);

    virtual double evalNet(Network *net);

    double MAX_FITNESS;
    bool MARKOV;

    bool N_MARKOV_LONG;                     //Flag that we are looking at the champ
    bool GENERALIZATION_TEST;               //Flag we are testing champ's generalization

    double state[6];

    double JIGGLE_STEP[1000];

protected:
    virtual void init();

private:

    void performAction(double output, int stepnum);

    void step(double action, double *state, double *derivs);

    void rk4(double f, double y[], double dydx[], double yout[]);

    bool outsideBounds();

	const static int NUM_INPUTS;
	const static double MUP;
	const static double GRAVITY;
	const static double MASS_CART;
	const static double MASS_POLE_1;
	const static double LENGTH_1;
	const static double FORCE_MAG;
	const static double TAU;
	const static double thirty_six_degrees;

    double LENGTH_2;
    double MASS_POLE_2;

    //Queues used for Gruau's fitness which damps oscillations
    int BALANCED_SUM;
    double CART_POS_SUM;
    double CART_V_SUM;
    double POLE_POS_SUM;
    double POLE_V_SUM;

};
#endif //RTNEAT_CARTPOLE_H
