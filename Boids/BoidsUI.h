#pragma once

#include "UI.h"

struct UIState
{

    float *cohesion, *alignment, *separation;
};

class BoidsUI : public UI
{

    UIState state;

public:

    BoidsUI(UIState state);

    virtual void draw();
};

