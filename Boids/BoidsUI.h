#pragma once

#include "UI.h"

struct UIState
{

    float *cohesion, *alignment, *separation;

    float frametime, computeTime, drawTime;
};

class BoidsUI : public UI
{

    UIState state;

    void drawBoids();

    void drawPerformance();

public:

    BoidsUI(UIState state);

    virtual void draw();

    // miliseconds
    void setFrametime(float frametime, float computeTime, float drawTime);
};

