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
    
    frametimeText << std::fixed << std::setprecision(1);
    frametimeText << "  Frame | Compute |  Draw\n";
    frametimeText << std::setw(5) << state.frametime << "ms |";
    frametimeText << std::setw(6) << state.computeTime << "ms |";
    frametimeText << std::setw(5) << state.drawTime << "ms";

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

// miliseconds
void BoidsUI::setFrametime(float frametime, float computeTime, float drawTime)
{

    state.frametime = frametime;

    state.computeTime = computeTime;

    state.drawTime = drawTime;
}
