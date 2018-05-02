//
// Created by magzhan on 1/9/17.
//

#include "CartPole.h"

const int CartPole::NUM_INPUTS = 7;
const double CartPole::MUP = 0.000002;
const double CartPole::GRAVITY = -9.8;
const double CartPole::MASS_CART = 1.0;
const double CartPole::MASS_POLE_1 = 0.1;
const double CartPole::LENGTH_1 = 0.5;         //actually half the pole's length
const double CartPole::FORCE_MAG = 10.0;
const double CartPole::TAU = 0.01;             //seconds between state updates
const double CartPole::thirty_six_degrees = 0.628329;

CartPole::CartPole(bool velocity) {
    MAX_FITNESS = 100000;
    MARKOV = velocity;
    LENGTH_2 = 0.05;
    MASS_POLE_2 = 0.01;
}

double CartPole::evalNet(Network *net) {
    int steps = 0;
    double input[NUM_INPUTS];
    double output;

    int nMarkovMax;

    double nMarkovFitness;

    double jiggleTotal; //total jiggle in last 100
    int count;          //step counter

    if (N_MARKOV_LONG) nMarkovMax = 100000;
    else nMarkovMax = 1000;

    init();

    if (MARKOV) {
        while (steps++ < MAX_FITNESS) {
            input[0] = state[0] / 4.8;
            input[1] = state[1] / 2;
            input[2] = state[2] / 0.52;
            input[3] = state[3] / 2;
            input[4] = state[4] / 0.52;
            input[5] = state[5] / 2;
            input[6] = .5;

            net->loadSensors(input);

            //Activate the net
            //If it loops, exit returning only fitness of 1 step
            if (!(net->activate())) return 1.0;

            output = (*(net->outputs.begin()))->activation;

            performAction(output, steps);

            if (outsideBounds())    // if failure stop it now
                break;
        }
        return (double) steps;
    } else {  //NON MARKOV CASE

        while (steps++ < nMarkovMax) {

            input[0] = state[0] / 4.8;
            input[1] = state[2] / 0.52;
            input[2] = state[4] / 0.52;
            input[3] = .5;

            net->loadSensors(input);

            //Activate the net
            //If it loops, exit returning only fitness of 1 step
            if (!(net->activate())) return 0.0001;

            output = (*(net->outputs.begin()))->activation;

            performAction(output, steps);

            if (outsideBounds())    // if failure  stop it now
                break;

            if (N_MARKOV_LONG && (outsideBounds()))    // if failure stop it now
                break;
        }

        //If we are generalizing we just need to balance it a while
        if (GENERALIZATION_TEST)
            return (double) BALANCED_SUM;

        //Sum last 100
        if ((steps > 100) && (!N_MARKOV_LONG)) {

            jiggleTotal = 0;
            cout << "step " << steps - 99 - 2 << " to step " << steps - 2 << endl;
            //Adjust for array bounds and count
            for (count = steps - 99 - 2; count <= steps - 2; count++)
                jiggleTotal += JIGGLE_STEP[count];
        }

        if (!N_MARKOV_LONG) {
            if (BALANCED_SUM > 100)
                nMarkovFitness = ((0.1 * (((double) BALANCED_SUM) / 1000.0)) +
                                  (0.9 * (0.75 / (jiggleTotal))));
            else nMarkovFitness = (0.1 * (((double) BALANCED_SUM) / 1000.0));

#ifndef NO_SCREEN_OUTR
            cout << "Balanced:  " << BALANCED_SUM << " jiggle: " << jiggleTotal << " ***" << endl;
#endif

            return nMarkovFitness;
        } else return (double) steps;

    }

}

void CartPole::init() {
    static int firstTime = 1;

    if (!MARKOV) {
        //Clear all fitness records
        CART_POS_SUM = 0.0;
        CART_V_SUM = 0.0;
        POLE_POS_SUM = 0.0;
        POLE_V_SUM = 0.0;
    }

    BALANCED_SUM = 0; //Always count # balanced

    if (!GENERALIZATION_TEST) {
        state[0] = state[1] = state[3] = state[4] = state[5] = 0;
        state[2] = 0.07; // one_degree;
    } else {
        state[4] = state[5] = 0;
    }

    if (firstTime) {
        cout << "Initial Long pole angle = %f\n" << state[2] << endl;;
        cout << "Initial Short pole length = %f\n" << LENGTH_2 << endl;
        firstTime = 0;
    }
}

void CartPole::performAction(double output, int stepnum) {

    int i;
    double dydx[6];

    //Runge-Kutta 4th order integration method

    //Apply action to the simulated cart-pole

    for (i = 0; i < 2; ++i) {
        dydx[0] = state[1];
        dydx[2] = state[3];
        dydx[4] = state[5];
        step(output, state, dydx);
        rk4(output, state, dydx, state);
    }

    //Record this state
    CART_POS_SUM += fabs(state[0]);
    CART_V_SUM += fabs(state[1]);
    POLE_POS_SUM += fabs(state[2]);
    POLE_V_SUM += fabs(state[3]);
    if (stepnum <= 1000)
        JIGGLE_STEP[stepnum - 1] = fabs(state[0]) + fabs(state[1]) + fabs(state[2]) + fabs(state[3]);

    if (!(outsideBounds()))
        ++BALANCED_SUM;

}

void CartPole::step(double action, double *st, double *derivs) {
    double force, cosTheta1, cosTheta2, sinTheta1, sinTheta2,
            gSinTheta1, gSinTheta2, temp1, temp2, ml_1, ml_2, fi_1, fi_2, mi_1, mi_2;

    force = (action - 0.5) * FORCE_MAG * 2;
    cosTheta1 = cos(st[2]);
    sinTheta1 = sin(st[2]);
    gSinTheta1 = GRAVITY * sinTheta1;
    cosTheta2 = cos(st[4]);
    sinTheta2 = sin(st[4]);
    gSinTheta2 = GRAVITY * sinTheta2;

    ml_1 = LENGTH_1 * MASS_POLE_1;
    ml_2 = LENGTH_2 * MASS_POLE_2;
    temp1 = MUP * st[3] / ml_1;
    temp2 = MUP * st[5] / ml_2;
    fi_1 = (ml_1 * st[3] * st[3] * sinTheta1) +
           (0.75 * MASS_POLE_1 * cosTheta1 * (temp1 + gSinTheta1));
    fi_2 = (ml_2 * st[5] * st[5] * sinTheta2) +
           (0.75 * MASS_POLE_2 * cosTheta2 * (temp2 + gSinTheta2));
    mi_1 = MASS_POLE_1 * (1 - (0.75 * cosTheta1 * cosTheta1));
    mi_2 = MASS_POLE_2 * (1 - (0.75 * cosTheta2 * cosTheta2));

    derivs[1] = (force + fi_1 + fi_2)
                / (mi_1 + mi_2 + MASS_CART);

    derivs[3] = -0.75 * (derivs[1] * cosTheta1 + gSinTheta1 + temp1)
                / LENGTH_1;
    derivs[5] = -0.75 * (derivs[1] * cosTheta2 + gSinTheta2 + temp2)
                / LENGTH_2;

}

void CartPole::rk4(double f, double y[], double dydx[], double yout[]) {

    int i;

    double hh, h6, dym[6], dyt[6], yt[6];


    hh = TAU * 0.5;
    h6 = TAU / 6.0;
    for (i = 0; i <= 5; i++) yt[i] = y[i] + hh * dydx[i];
    step(f, yt, dyt);
    dyt[0] = yt[1];
    dyt[2] = yt[3];
    dyt[4] = yt[5];
    for (i = 0; i <= 5; i++) yt[i] = y[i] + hh * dyt[i];
    step(f, yt, dym);
    dym[0] = yt[1];
    dym[2] = yt[3];
    dym[4] = yt[5];
    for (i = 0; i <= 5; i++) {
        yt[i] = y[i] + TAU * dym[i];
        dym[i] += dyt[i];
    }
    step(f, yt, dyt);
    dyt[0] = yt[1];
    dyt[2] = yt[3];
    dyt[4] = yt[5];
    for (i = 0; i <= 5; i++)
        yout[i] = y[i] + h6 * (dydx[i] + dyt[i] + 2.0 * dym[i]);
}

bool CartPole::outsideBounds() {
    const double failureAngle = thirty_six_degrees;

    return
            state[0] < -2.4 ||
            state[0] > 2.4 ||
            state[2] < -failureAngle ||
            state[2] > failureAngle ||
            state[4] < -failureAngle ||
            state[4] > failureAngle;
}
