#include "BoidsUI.h"

BoidsUI::BoidsUI(UIState state)
    : state(state)
{
}

void BoidsUI::draw()
{

    ImGui::Begin("Boids");

    ImGui::SliderFloat("Cohesion", &(state.cohesion), 0.0, 5.0);

    ImGui::SliderFloat("Alignment", &(state.alignment), 0.0, 5.0);

    ImGui::SliderFloat("Separation", &(state.separation), 0.0, 5.0);

    ImGui::End();
}

UIState BoidsUI::getState() const
{
    return state;
}
