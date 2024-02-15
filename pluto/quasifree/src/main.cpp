#include "../include/file_reader.h"
#include "../include/event_generator.h"
#include "../include/data_writer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <TGraph.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Model Name>" << std::endl;
        return 1;
    }
    std::string model_name = argv[1];
    std::map<std::string, std::string> model_to_file_map;
    model_to_file_map["paris"] = "../momentum_distributions/paris_momentum_distribution.txt";
    model_to_file_map["cdbonn"] = "../momentum_distributions/cdbonn_momentum_distribution.txt";

    

    std::string file_path = model_to_file_map[model_name];
    if (file_path.empty()) {
        std::cerr << "Error: No file associated with model " << model_name << std::endl;
        return 1;
    }

    FileReader fileReader(file_path);
    TGraph* graph = fileReader.read_data();


    const int num_iterations = 5;
    for (int iteration = 1; iteration <= num_iterations; ++iteration) {
        EventGenerator eventGenerator;
        eventGenerator.setGraph(graph);

        // Generate events
        eventGenerator.generatePlot();

        // Format output file name
        std::ostringstream file_name_stream;
        file_name_stream << "${PLUTO_OUTPUT}/pd-ppn_spec-" << model_name << "-" << iteration << ".root";
        std::string file_name = file_name_stream.str();

        std::vector<std::pair<double, double> > calculated_data = eventGenerator.getCalculatedData();;

        // Generate a text file name similar to the ROOT file name
        std::ostringstream text_file_name_stream;
        text_file_name_stream << "../proton_momentum_theta/proton_momentum_theta-" << model_name << "-" << iteration << ".txt";
        std::string text_file_name = text_file_name_stream.str();
        
        // Write results
        DataWriter dataWriter(file_name);
        dataWriter.writeTree(graph);

        // Write the calculated data to a text file
        dataWriter.writeProtonData(calculated_data, text_file_name);
    }

    // Cleanup
    delete graph;

    return 0;
}
