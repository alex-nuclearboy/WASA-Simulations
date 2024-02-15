#include "../include/event_generator.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include <iostream>

EventGenerator::EventGenerator() : graph(nullptr) {}

void EventGenerator::setGraph(TGraph* graph) {
    this->graph = graph;
}

void EventGenerator::generatePlot() 
{
    if (!graph) {
        std::cerr << "Graph not set." << std::endl;
        return;
    }

    int n = graph->GetN();
    double x, y;
    calculated_data.clear();
    for (int i = 0; i < n; ++i) {
        graph->GetPoint(i, x, y);
        double newMomentum = x + 0.01; // Example calculation
        double newProbability = y; // Example new probability calculation
        calculated_data.push_back(std::make_pair(newMomentum, newProbability));
    }

    TCanvas* canvas = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c1");
    if (!canvas) {
        canvas = new TCanvas("c1", "Event Generator Plot", 200, 10, 700, 500);
    } else {
        canvas->Clear();
    }

    if (graph) {
        canvas->cd(); // Make the canvas the current pad
        graph->Draw("AP"); // Draw the graph on the canvas
    } else {
        std::cerr << "Graph not set." << std::endl;
    }
    
}

std::vector<std::pair<double, double>> EventGenerator::getCalculatedData() 
{
    return calculated_data;
}
