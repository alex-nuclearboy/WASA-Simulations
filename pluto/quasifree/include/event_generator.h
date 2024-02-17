#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H

#include <string>
#include <vector>
#include "TGraph.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "data_writer.h"

struct ParticleData {
    std::string name;
    TLorentzVector vector;
    ParticleData(const std::string& n, const TLorentzVector& v)
        : name(n), vector(v) {}
};

class EventGenerator {
public:
    EventGenerator(
        TGraph* graph, DataWriter& writer, const std::string& particlesFile, 
        const std::string& calculationsFile, const std::string& protonDataFile);
    ~EventGenerator();
    void setGraph(TGraph* graph);
    void generateEvents(int num_events);

    std::vector<std::pair<Double_t, Double_t> > getProtonData() const { return proton_data; }

    TTree* getParticlesTree() const { return tree_; }
    //TTree* getCalculationsTree() const { return calculations_tree_; }

private:
    void setupTree();
    void cleanup();

    DataWriter& writer_;

    std::vector<std::pair<double, double> > proton_data;

    std::string particlesFileName_;
    std::string calculationsFileName_;
    std::string protonDataFileName_;

    TTree* particlesTree_;
    TTree* calculationsTree_;
    
    
    void setParticles(TClonesArray* particlesArray, const std::vector<ParticleData>& particlesData);

    std::string file_name_;
    
    TGraph* graph_;
    TTree* tree_;
    TClonesArray* particles_;
 
    Int_t   Npart_;
    Float_t Impact_;
    Float_t Phi_;

    Double_t momentum_;

    

};

#endif // EVENT_GENERATOR_H
