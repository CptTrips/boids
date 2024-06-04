#include "BoidsUI.h"

#include <string>
#include <sstream>
#include <iomanip>

void BoidsUI::drawBoids()
{

    ImGui::Begin("Boids");

    ImGui::SliderFloat("Cohesion", state.cohesion, 0.0, 5.0);

    ImGui::SliderFloat("Alignment", state.alignment, 0.0, 5.0);

    ImGui::SliderFloat("Separation", state.separation, 0.0, 5.0);

    ImGui::End();
}

void BoidsUI::drawPerformance()
{

    ImGui::Begin("Performance");

    std::stringstream frametimeText;
    
    frametimeText << std::fixed << std::setprecision(1) << state.frametime / 1e6 << "ms";

    ImGui::Text(frametimeText.str().c_str());

    ImGui::End();
}

BoidsUI::BoidsUI(UIState state)
    : state(state)
{
}

void BoidsUI::draw()
{

    drawBoids();

    drawPerformance();
}

void BoidsUI::setFrametime(float frametime)
{

    state.frametime = frametime;
}
