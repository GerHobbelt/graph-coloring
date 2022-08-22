#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <gflags/gflags.h>

#include "../Header/parse.hpp"
#include "../Header/coloring_algorithm.hpp"
#include "../Header/dsatur.hpp"
#include "../Header/mcs.hpp"
#include "../Header/lmxrlf.hpp"
#include "../Header/hybrid_dsatur.hpp"
#include "../Header/hybrid_lmxrlf.hpp"

using std::cerr;
using std::endl;

using GraphColoring::Dsatur;
using GraphColoring::Mcs;
using GraphColoring::Mcs2;
using GraphColoring::Lmxrlf;
using GraphColoring::HybridDsatur;
using GraphColoring::HybridLmxrlf;
using GraphColoring::GraphColor;
using GraphColoring::GraphColor2;

DEFINE_string(graph, "", "The path to the graph file to be colored");
DEFINE_string(algorithm, "mcs", "The algorithm to execute on chosen benchmark (dsatur, mcs, lmxrlf, hybrid dsatur, hybrid lmxrlf)");
DEFINE_string(format, "", "The format of the input graph to be parsed (matrix, list)");
DEFINE_string(output, "", "The output json file that stored the node categorized by color");

GraphColor* parse_algorithm_flag(map<string,vector<string>> graph) {
    if(FLAGS_algorithm == "dsatur") {
        return new Dsatur(graph);
    } else if(FLAGS_algorithm == "mcs") {
        return new Mcs(graph);
    } else if(FLAGS_algorithm == "lmxrlf") {
        return new Lmxrlf(graph);
    } else if(FLAGS_algorithm == "hybrid-dsatur") {
        return new HybridDsatur(graph);
    } else if(FLAGS_algorithm == "hybird-lmxrlf") {
        return new HybridLmxrlf(graph);
    }
    return nullptr;
}

GraphColor2* parse_algorithm_flag2(vector<vector<int>>& graph) {
    if (FLAGS_algorithm == "mcs") {
        return new Mcs2(graph);
    }

    return nullptr;
}

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    string banner = "This program attempts to color an input graph using one of the available coloring algorithms";
    string usage = "\tUsage: ./color --graph=path_to_graph --format=(m[atrix]|l[ist]) [--algorithm=(dsatur|mcs|lmxrlf|hybrid-dsatur|hybrid-lmxrlf)]";
    gflags::SetUsageMessage(banner + "\n" + usage);

    if(FLAGS_graph == "") {
        gflags::ShowUsageWithFlags(argv[0]);
        return -1;
    }

    vector<vector<int>> input_graph;
    if(FLAGS_format == "matrix" || FLAGS_format == "m") {
        //input_graph = parse_edge_matrix(FLAGS_graph);
        std::cout << "Matrix not supported for graph coloring 2!";
    } else if(FLAGS_format == "list" || FLAGS_format == "l") {
      parse_edge_list(FLAGS_graph, input_graph);
    } else {
        gflags::ShowUsageWithFlags(argv[0]);
        return -1;
    }

    GraphColor2 *graph = parse_algorithm_flag2(input_graph);
    if(!graph) {
        gflags::ShowUsageWithFlags(argv[0]);
        return -1;
    }

    if(input_graph.size() == 0) { 
        cerr << "Stopping due to failure to parse input file" << endl;
        return -2; 
    }

    graph->color();
    graph->print_chromatic();
    //graph->print_coloring();

    if(!graph->is_valid()) {
        cerr << "Graph coloring is invalid" << endl;
        return -1;
    }

    if (FLAGS_output.size() != 0)
    {
        graph->saveColoringCategories(FLAGS_output);
    }

    return 0;
}

