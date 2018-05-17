#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include <functional>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>
#include "Defines.hpp"

class Trajectory
{
protected:
    float totalTime = 0.0;
    std::function<float(float)> calc_ax,calc_ay;
    bool active_ax = false ,active_ay = false;
public:
    Trajectory(const std::function<float(float)>& calc_ax, const std::function<float(float)>& calc_ay);
    Trajectory() = default;
    inline void setTrajectoryX( const std::function<float(float)>& f) { calc_ax = f;active_ax=true; }
    inline void setTrajectoryY( const std::function<float(float)>& f) { calc_ay = f;active_ay=true; }
    virtual inline void update(float timeElapsed) { totalTime+=timeElapsed; }
    inline float getAx() const { return calc_ax(totalTime); }
    inline float getAy() const { return calc_ay(totalTime); }
    inline bool isActiveAx() const { return active_ax; }
    inline bool isActiveAy() const { return active_ay; }
    inline void setActiveAx(bool b) { active_ax=b; }
    inline void setActiveAy(bool b) { active_ay=b; }
};

class Cos_Trajectory : public Trajectory
{
private:
    float ampX;
    float pulseX;
    float phaseX;
    float ampY;
    float pulseY;
    float phaseY;
public:
    Cos_Trajectory(float ampX,float pulseX,float phaseX,float ampY,float pulseY,float phaseY);
};

#endif //TRAJECTORY_HPP
