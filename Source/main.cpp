#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <gflags/gflags.h>

#include "../Header/parse.hpp"
#include "../Header/coloring_algorithm.hpp"
#include "../Header/dsatur.hpp"
#include "../Header/mcs.hpp"
#include "../Header/greedy.hpp"
#include "../Header/lmxrlf.hpp"
#include "../Header/hybrid_dsatur.hpp"
#include "../Header/hybrid_lmxrlf.hpp"
#include "json.hpp"

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
using GraphColoring::GraphClustering;
using GraphColoring::OrderedGreedy;

DEFINE_string(graph, "", "The path to the graph file to be colored");
DEFINE_string(weights, "", "The weights of each edge in the graph, only used for clustering");
DEFINE_int32(clusterSize, 5, "");
DEFINE_string(algorithm, "mcs", "The algorithm to execute on chosen benchmark (dsatur, mcs, lmxrlf, hybrid dsatur, hybrid lmxrlf)");
DEFINE_string(format, "", "The format of the input graph to be parsed (matrix, list)");
DEFINE_string(output, "", "The output json file that stored the node categorized by color");
DEFINE_bool(balanceGraph, true, "Use post processing to balance the size of each color");

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
    else if (FLAGS_algorithm == "greedy")
    {
        return new OrderedGreedy(graph);

    }

    return nullptr;
}

int main(int argc, const char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    string banner = "This program attempts to color an input graph using one of the available coloring algorithms";
    string usage = "\tUsage: ./color --graph=path_to_graph --format=(m[atrix]|l[ist]) [--algorithm=(dsatur|mcs|lmxrlf|hybrid-dsatur|hybrid-lmxrlf)]";
    gflags::SetUsageMessage(banner + "\n" + usage);

    if(FLAGS_graph == "") {
        gflags::ShowUsageWithFlags(argv[0]);
        return -1;
    }

    
    if(FLAGS_weights != "")
    // clustered coloring
    {
        std::vector<float> weights;
        std::ifstream ifWeights(FLAGS_weights);
        nlohmann::json weightsJ = nlohmann::json::parse(ifWeights);
        weights = weightsJ.get<std::vector<float>>();

        vector<vector<int>> input_graph;
        vector<vector<float>> input_graph_weights;
        if (FLAGS_format == "list" || FLAGS_format == "l") {
            parse_edge_list_with_weights(FLAGS_graph, weights, input_graph, input_graph_weights);
        }
        else {
            gflags::ShowUsageWithFlags(argv[0]);
            return -1;
        }

        GraphClustering clustering(input_graph, input_graph_weights);

        clustering.cluster(FLAGS_clusterSize);

        GraphColor2* graphForColoring = new Mcs2(clustering.clusteredGraph);
        std::vector<int> coloring = graphForColoring->color();
        graphForColoring->print_chromatic();

        if (!graphForColoring->is_valid()) {
            cerr << "Graph coloring is invalid" << endl;
            return -1;
        }


        if (FLAGS_output.size() != 0)
        {
            clustering.saveClusteredColoringCategories(graphForColoring->get_num_colors(), coloring, FLAGS_output);
        }
    }
    else
        // graph clustering
    {
        if (FLAGS_algorithm != "mcs"
            && FLAGS_algorithm != "greedy") {
            map<string, vector<string>> input_graph;
            if (FLAGS_format == "matrix" || FLAGS_format == "m") {
                input_graph = parse_edge_matrix(FLAGS_graph);
            }
            else if (FLAGS_format == "list" || FLAGS_format == "l") {
                input_graph = parse_edge_list(FLAGS_graph);
            }
            else {
                gflags::ShowUsageWithFlags(argv[0]);
                return -1;
            }

            GraphColor* graph = parse_algorithm_flag(input_graph);
            if (!graph) {
                gflags::ShowUsageWithFlags(argv[0]);
                return -1;
            }

            if (input_graph.size() == 0) {
                cerr << "Stopping due to failure to parse input file" << endl;
                return -2;
            }

            graph->color();
            graph->print_chromatic();
            if (!graph->is_valid()) {
                cerr << "Graph coloring is invalid" << endl;
                return -1;
            }
        }
        else
        {
            vector<vector<int>> input_graph;
            if (FLAGS_format == "matrix" || FLAGS_format == "m") {
                //input_graph = parse_edge_matrix(FLAGS_graph);
                std::cout << "Matrix not supported for graph coloring 2!";
            }
            else if (FLAGS_format == "list" || FLAGS_format == "l") {
                parse_edge_list(FLAGS_graph, input_graph);
            }
            else {
                gflags::ShowUsageWithFlags(argv[0]);
                return -1;
            }

            GraphColor2* graph = parse_algorithm_flag2(input_graph);
            if (!graph) {
                gflags::ShowUsageWithFlags(argv[0]);
                return -1;
            }
            if (input_graph.size() == 0) {
                cerr << "Stopping due to failure to parse input file" << endl;
                return -2;
            }

            graph->color();
            graph->print_chromatic();
            //graph->print_coloring();

            if (!graph->is_valid()) {
                cerr << "Graph coloring is invalid" << endl;
                return -1;
            }
            
            graph->convertToColoredCategories();

            if (FLAGS_balanceGraph)
            {
                graph->balanceColoredCategories(1.01);
                if (!graph->is_valid()) {
                    cerr << "Graph coloring is invalid" << endl;
                    return -1;
                }
            }


            if (FLAGS_output.size() != 0)
            {
                graph->saveColoringCategories(FLAGS_output);
            }

        }
    }
    


    return 0;
}

