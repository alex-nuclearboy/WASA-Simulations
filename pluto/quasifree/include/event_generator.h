#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include "TGraph.h"

class EventGenerator {
public:
    EventGenerator();
    void setGraph(TGraph* graph);
    void generatePlot();
    std::vector<std::pair<double, double> > getCalculatedData();

private:
    TGraph* graph;
    std::vector<std::pair<double, double> > calculated_data;
};

#endif // EVENTGENERATOR_H
