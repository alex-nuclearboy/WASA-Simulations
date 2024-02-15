#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H

#include <string>
#include <vector>
#include "TGraph.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

class EventGenerator {
public:
    explicit EventGenerator(const std::string& file_name);
    ~EventGenerator();
    void setGraph(TGraph* graph);
    void calculate();
    void generateEvents(int num_events);
    std::vector<std::pair<double, double> > getCalculatedData() const;

private:
    void setupTree();
    void cleanup();

    TGraph* graph_;
    std::string file_name_;
    TTree* tree_;
    TClonesArray* particles_;
    std::vector<std::pair<double, double> > calculated_data_;

    static const double PROTON_MASS;
    static const double NEUTRON_MASS;
    static const double DEUTERON_MASS;

    TLorentzVector finalStateParticles[2];

    Int_t   Npart_;
    Float_t Impact_;
    Float_t Phi_;

};

#endif // EVENT_GENERATOR_H
