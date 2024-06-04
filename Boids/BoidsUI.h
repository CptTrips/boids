#pragma once

#include "UI.h"

struct UIState
{

    float *cohesion, *alignment, *separation;

    float frametime;
};

class BoidsUI : public UI
{

    UIState state;

    void drawBoids();

    void drawPerformance();

public:

    BoidsUI(UIState state);

    virtual void draw();

    void setFrametime(float frametime);
};

