
#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <list>
#include "../Header/mcs.hpp"
#include <chrono>       // std::chrono::system_clock
#include <random>

using std::queue;
using std::cout;
using std::cerr;
using std::endl;

// TODO: The following optimization can be made via a note in the paper
// The procedure MCS can be implemented to run in O(|V | + |E|) time. To 
// see that, notice that the first loop executes |V | iterations. In the 
// second loop, for each vertex of G, all its neighbors are visited. After 
// a vertex is evaluated, it is removed from the remaining graph. Therefore, 
// the weight λ is increased exactly |E| times. By keeping vertices in an 
// array of buckets indexed by λ, the vertex of highest weight can be found 
// in O(1) time.

//Maximum Cardinal Search
map<string,int> GraphColoring::Mcs::color() {

    map<string,vector<string>> temp_graph = graph;
    map<string,int> weight;
    queue<string> ordering;

    // Initially set the weight of each node to 0
    for(map<string,vector<string>>::iterator i = temp_graph.begin(); i != temp_graph.end(); i++) {
        weight[i->first] = 0;
    }

    // Work through all the nodes in the graph, choosing the node
    // with maximum weight, then add that node to the queue. Increase
    // the weight of the queued nodes neighbors by 1. Continue until
    // every node in the graph has been added to the queue
    for(int i = 0; i < this->graph.size(); i++) {
        int max_weight = -1;
        string max_vertex = "";

        // Out of the remaining nodes, find the node with the highest weight
        for(map<string,vector<string>>:: iterator j = temp_graph.begin(); j != temp_graph.end(); j++) {
            if(weight[j->first] > max_weight) {
                max_weight = weight[j->first];
                max_vertex = j->first;
            }
        }
        if(max_vertex == "") {
            cerr << "Error: Could not find a max weight node in the graph (reason unknown)" << endl;
            this->graph_colors = map<string,int>();
            return this->graph_colors;
        }

        // Add highest weight node to the queue and increment all of its
        // neighbors weights by 1
        ordering.push(max_vertex);
        for(unsigned j = 0; j < graph[max_vertex].size(); j++) {
            weight[temp_graph[max_vertex][j]] += 1;
        }

        // Remove the maximum weight node from the graph so that it won't
        // be accidentally added again
        temp_graph.erase(max_vertex);
    }

    // Work through the queue in order and color each node
    while(!ordering.empty()) {
        int color = 0;

        // Find the lowest possible graph_colors for this node between
        // its neighbors
        string min = ordering.front();

        //Thanks to Michael Kochte @ Universitaet Stuttgart for the below speedup snippit

        //Collect color numbers of neighbors
        std::set<int> colorset;
        for(unsigned i = 0; i < graph[min].size(); i++) {
            int col = graph_colors[graph[min][i]];
            colorset.insert(col);
        }

        //Sort and uniquify
        vector<int> colorvec;
        std::copy(colorset.begin(), colorset.end(), std::back_inserter(colorvec));
        std::sort(colorvec.begin(), colorvec.end());

        //Pick the lowest color not contained
        int newcolor = 0;
        for(unsigned i = 0; i < colorvec.size(); i++) {
            if(colorvec[i] == newcolor) {
                newcolor++;
            }
        }
        color = newcolor;

        this->graph_colors[min] = color;
        ordering.pop();
    }   
    return this->graph_colors;
}

vector<int>& GraphColoring::Mcs2::color()
{
    //std::list<int> temp_graph;
    std::vector<int> temp_graph;
    for (size_t i = 0; i < graph.size(); i++)
    {
        temp_graph.push_back(i);
    }
    

    vector<int> weight(temp_graph.size());
    queue<int> ordering;

    std::cout << "Initializing.\n";

    // Initially set the weight of each node to 0
    for (int i = 0; i < weight.size(); ++i) {
        weight[i] = 0;
    }

    std::cout << "Working through all the nodes in the graph to update maximum weight.\n";

    // Work through all the nodes in the graph, choosing the node
    // with maximum weight, then add that node to the queue. Increase
    // the weight of the queued nodes neighbors by 1. Continue until
    // every node in the graph has been added to the queue

    int percentage = 0;

    std::vector<int> coloringOrder(this->graph.size());
    for (int iNode = 0; iNode < this->graph.size(); iNode++) {
        coloringOrder[iNode] = iNode;

    }


    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(std::begin(coloringOrder), std::end(coloringOrder), e);

    for (int i = 0; i < this->graph.size(); i++) {
        int iNode = coloringOrder[i];

        int max_weight = -1;
        int max_vertex = -1;

        // Out of the remaining nodes, find the node with the highest weight
        ;
        int maxWId;
        for (int j = 0; j < temp_graph.size(); j++) {
            int nodeId = temp_graph[j];
            if (nodeId < 0)
            {
                continue;
            }
            if (weight[nodeId] > max_weight) {
                max_weight = weight[nodeId];
                max_vertex = nodeId;
                maxWId = j;
            }
        }
        if (max_vertex == -1) {
            cerr << "Error: Could not find a max weight node in the graph (reason unknown)" << endl;
            this->graph_colors = vector<int>();
            return graph_colors;
        }

        // Add highest weight node to the queue and increment all of its
        // neighbors weights by 1
        ordering.push(max_vertex);
        for (unsigned j = 0; j < graph[max_vertex].size(); j++) {
            weight[graph[max_vertex][j]] += 1;
        }

        // Remove the maximum weight node from the graph so that it won't
        // be accidentally added again
        //temp_graph.erase(graphIterMaxW); // 73010 used 3:21
        //*graphIterMaxW = -1;             // 73010 used 2:31
        temp_graph[maxWId] = -1;                // 73010 used 2:10

        //std::cout << i << "  th iteration.\n";
        if (100.0 * (double)iNode / this->graph.size() > percentage)
        {
            std::cout << percentage << "%  finished." << endl;
            percentage += 1;

            // update temp_graph to remove negative ids
            std::vector<int> temp_graph_new;
            for (size_t j = 0; j < temp_graph.size(); j++)
            {
                if (temp_graph[j] > 0)
                {
                    temp_graph_new.push_back(temp_graph[j]);
                }
            }
            temp_graph = std::move(temp_graph_new);

        }

    }

    int sizeOrdering = ordering.size();
    percentage = 0;
    std::cout << "Work through the queue in order and color each node.\n";

    // Work through the queue in order and color each node
    while (!ordering.empty()) {
        int color = 0;

        // Find the lowest possible graph_colors for this node between
        // its neighbors
        int min = ordering.front();

        //Thanks to Michael Kochte @ Universitaet Stuttgart for the below speedup snippit

        //Collect color numbers of neighbors
        std::set<int> colorset;
        for (unsigned i = 0; i < graph[min].size(); i++) {
            int col = graph_colors[graph[min][i]];
            colorset.insert(col);
        }

        //Sort and uniquify
        vector<int> colorvec;
        std::copy(colorset.begin(), colorset.end(), std::back_inserter(colorvec));
        std::sort(colorvec.begin(), colorvec.end());

        //Pick the lowest color not contained
        int newcolor = 0;
        for (unsigned i = 0; i < colorvec.size(); i++) {
            if (colorvec[i] == newcolor) {
                newcolor++;
            }
        }
        color = newcolor;

        this->graph_colors[min] = color;
        ordering.pop();

        if (100.0 * (double)(sizeOrdering - ordering.size()) / sizeOrdering > percentage)
        {
            std::cout << percentage << "%  finished." << endl;;
            percentage += 1;

        }

    }
    return this->graph_colors;
}