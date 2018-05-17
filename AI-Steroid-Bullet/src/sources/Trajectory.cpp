//
// Created by asphox on 17/05/18.
//

#include "../headers/Trajectory.hpp"

Trajectory::Trajectory(const std::function<float(float)> &calc_ax, const std::function<float(float)> &calc_ay) :
    calc_ax(calc_ax),
    calc_ay(calc_ay),
    active_ax(true),
    active_ay(true)
{
}

Cos_Trajectory::Cos_Trajectory(float _ampX, float _pulseX, float _phaseX, float _ampY, float _pulseY , float _phaseY) :
    Trajectory(),
    ampX(_ampX),
    pulseX(_pulseX),
    phaseX(_phaseX),
    ampY(_ampY),
    pulseY(_pulseY),
    phaseY(_phaseY)

{
    if(ampX != 0)
        setTrajectoryX([this](float t){ return ampX*cos(pulseX*t+phaseX); });
    if(ampY != 0)
        setTrajectoryY([this](float t){ return ampY*cos(pulseY*t+phaseY); });
}