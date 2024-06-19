#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>

#include "../Header/coloring_algorithm.hpp"
#include "json.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ostringstream;

GraphColoring::GraphColor::GraphColor() {
    this->graph = map<string,vector<string>>();
    this->graph_colors = map<string,int>();
}

GraphColoring::GraphColor::GraphColor(map<string,vector<string>> graph) {
    this->graph = graph;
    for(map<string,vector<string>>::iterator itr = graph.begin(); itr != graph.end(); itr++) {
        this->graph_colors[itr->first] = -1;
    }
}

// Checks that no two adjacent nodes have the same color
bool GraphColoring::GraphColor::is_valid() {
    if(this->graph_colors.size() == 0 || this->graph.size() != this->graph_colors.size()) {
        return false;
    }
    for(map<string,vector<string>>::iterator i = this->graph.begin(); i != this->graph.end(); i++) {
        if(this->graph_colors.find(i->first) == this->graph_colors.end()) {
            return false;
        }
        for(unsigned j = 0; j < i->second.size(); j++) {
            if(this->graph_colors[i->first] == this->graph_colors[i->second[j]] || this->graph_colors[i->first] == -1) {
                return false;
            }
        }
    }
    return true;
}

// Returns the color of an individual node, if that node isn't colored it returns -1
int GraphColoring::GraphColor::get_color(string node) {
    if(this->graph_colors.find(node) != this->graph_colors.end()) {
        return this->graph_colors.at(node);
    }
    return -1;
}

// Used to print the Chromatic Color
void GraphColoring::GraphColor::print_chromatic() {
    cout << this->get_algorithm() << " Chromatic Number: " << this->get_num_colors() << endl;

    vector<int> categoriesSize(this->get_num_colors());

    //for (int iV = 0; iV < size(); iV++) {
    //    int color = get_color(std::to_string(iV+1));
    //    // in this 
    //    categories[color-1].push_back(iV);
    //}

    for (map< string, int >::iterator itr = graph_colors.begin(); itr != graph_colors.end(); itr++) {
        // std::cout << itr->first << " " << itr->second << endl;
        categoriesSize[itr->second]++;
    }

    for (size_t iColor = 0; iColor < this->get_num_colors(); iColor++)
    {
        cout << "num nodes in colored category " << iColor << ": " << categoriesSize[iColor] << std::endl;
    }
}

// Used to print the color of each node in the graph
void GraphColoring::GraphColor::print_coloring() {
    std::cout << "----------" << this->get_algorithm() << " Colorings----------" << endl;
    for(map< string,int >::iterator itr = graph_colors.begin(); itr != graph_colors.end(); itr++) {
        std::cout << itr->first << " " << itr->second << endl;
    }
}

int GraphColoring::GraphColor::get_num_colors() {
    int max_color = 0;
    for(map<string,int>::iterator color_it = this->graph_colors.begin(); color_it != this->graph_colors.end(); color_it++) {
        if(color_it->second > max_color) {
            max_color = color_it->second;
        }
    }
    return max_color + 1;
}

void GraphColoring::GraphColor::saveColoringCategories(std::string outputFile) {
    size_t numColors = get_num_colors();
    vector<vector<int>> categories(numColors);

    for (int iV = 0; iV < size(); iV++) {
        string node = "v";
        string temp;
        std::ostringstream convert;
        convert << (iV + 1);
        temp = convert.str();
        node.append(temp);

        int color = get_color(node);
        // in this 
        categories[color].push_back(iV);
    }

    nlohmann::json j= categories;
    
    std::ofstream ofs(outputFile);
    ofs << j.dump();
}


bool GraphColoring::GraphColor::is_colored() {
    if(this->graph_colors.size() < 1) {
        return false;
    } else {
        for(map<string,int>::iterator itr = this->graph_colors.begin(); itr != this->graph_colors.end(); itr++) {
            if(itr->second == -1) {
                return false;
            }
        }
        return true;
    }
}

GraphColoring::GraphColor2::GraphColor2()
{
}

GraphColoring::GraphColor2::GraphColor2(const std::vector<vector<int>>& inGraph)
{
    graph = inGraph;
    for (size_t i = 0; i < graph.size(); i++)
    {
        graph_colors.push_back(-1);
    }

}

bool GraphColoring::GraphColor2::is_colored()
{
    for (size_t i = 0; i < graph.size(); i++)
    {
        if (graph_colors[i] == -1)
        {
            return false;
        }
    }
    return true;
}

int GraphColoring::GraphColor2::get_color(int node)
{
    return graph_colors[node];
}

int GraphColoring::GraphColor2::get_num_colors()
{
    int numColors = 0;
    for (size_t i = 0; i < graph.size(); i++)
    {
        if (graph_colors[i]+1 > numColors )
        {
            numColors = graph_colors[i]+1;
        }
    };
    return numColors;
}

bool GraphColoring::GraphColor2::is_valid()
{
    if (this->graph_colors.size() == 0 || this->graph.size() != this->graph_colors.size()) {
        return false;
    }
    for (size_t i = 0; i < graph.size(); i++) {
        if (graph_colors[i] == -1) {
            return false;
        }
        for (size_t j = 0; j < graph[i].size(); j++) {
            int neiNode = graph[i][j];
            if (graph_colors[i] == graph_colors[neiNode]) {
                return false;
            }
        }
    }
    return true;
}

void GraphColoring::GraphColor2::print_chromatic()
{
    cout << this->get_algorithm() << " Chromatic Number: " << this->get_num_colors() << endl;
    //vector<int> categoriesSize(this->get_num_colors());

    //for (int iV = 0; iV < size(); iV++) {
    //    int color = get_color(std::to_string(iV+1));
    //    // in this 
    //    categories[color-1].push_back(iV);
    //}

    //for (int iV = 0; iV < graph_colors.size(); iV++) {
    //    // std::cout << itr->first << " " << itr->second << endl;
    //    categoriesSize[graph_colors[iV]]++;
    //}

    //for (size_t iColor = 0; iColor < this->get_num_colors(); iColor++)
    //{
    //    cout << "num nodes in colored category " << iColor << ": " << categoriesSize[iColor] << std::endl;
    //}
}

void GraphColoring::GraphColor2::convertToColoredCategories()
{
    size_t numColors = get_num_colors();
    categories.resize(numColors);

    for (int iV = 0; iV < size(); iV++) {
        int color = get_color(iV);
        // in this 
        categories[color].push_back(iV);
    }
}

int GraphColoring::GraphColor2::findChangableNodeInCategory(int sourceColor, int destinationColor)
{
    auto& sourceCategory = categories[sourceColor];
    for (size_t iNode = 0; iNode < sourceCategory.size(); iNode++)
    {
        if (changable(sourceCategory[iNode], destinationColor)) {
            return iNode;
        }
    }
    return -1;
}

void GraphColoring::GraphColor2::changeColor(int sourceColor, int categoryId, int destinationColor)
{
    int nodeId = categories[sourceColor][categoryId];
    graph_colors[nodeId] = destinationColor;

    if (categories.size())
    {
        categories[sourceColor].erase(categories[sourceColor].begin() + categoryId);
        categories[destinationColor].push_back(nodeId);
    }
}

bool GraphColoring::GraphColor2::changable(int node, int destinationColor)
{
    // loop through node and see if it has destinationColor
    for (size_t i = 0; i < graph[node].size(); i++)
    {
        int neiId = graph[node][i];
        if (graph_colors[neiId] == destinationColor)
        {
            return false;
        }
    }
    return true;
}

float GraphColoring::GraphColor2::findLargestSmallestCategories(int& biggestCategory, int& smallestCategory)
{
    if (categories.size()==0)
    {
        biggestCategory = -1;
        smallestCategory = -1;

        return 1;
    }

    size_t maxSize = categories[0].size();
    biggestCategory = 0;
    size_t minSize = categories[0].size();
    smallestCategory = 0;

    for (size_t iColor = 0; iColor < categories.size(); iColor++)
    {
        if (maxSize < categories[iColor].size()) {
            biggestCategory = iColor;
            maxSize = categories[iColor].size();
        }

        if (minSize > categories[iColor].size())
        {
            smallestCategory = iColor;
            minSize = categories[iColor].size();
        }
    }

    return float(categories[biggestCategory].size()) / float(categories[smallestCategory].size());
}

void GraphColoring::GraphColor2::balanceColoredCategories(float goalMaxMinRatio)
{
    float maxMinRatio = -1.f;

    do 
    {
        int biggestCategory = -1, smallestCategory = -1;

        maxMinRatio = findLargestSmallestCategories(biggestCategory, smallestCategory);

        // find a availiable vertex from the biggest category to move to the smallest category
        int changableId = findChangableNodeInCategory(biggestCategory, smallestCategory);
        if (changableId == -1)
        {
            for (size_t iColor = 0; iColor < categories.size(); iColor++)
            {
                if (iColor == biggestCategory || iColor == smallestCategory)
                {
                    continue;
                }

                changableId = findChangableNodeInCategory(iColor, smallestCategory);

                if (changableId = -1)
                {
                    biggestCategory = iColor;

                    break;
                }
            }
        }

        changeColor(biggestCategory, changableId, smallestCategory);

        if (changableId == -1)
        {
            std::cout << "The graph is not opimizable anymore, terminated with a max/min ratio: " << maxMinRatio << std::endl;
            return;
        }

        // change the color of changable id

    } while (maxMinRatio > goalMaxMinRatio);

    std::cout << "The graph optimization terminated with a max/min ratio: " << maxMinRatio << std::endl;

}

void GraphColoring::GraphColor2::saveColoringCategories(std::string outputFile)
{
    

    for (size_t iColor = 0; iColor < this->get_num_colors(); iColor++)
    {
        cout << "num nodes in colored category " << iColor << ": " << categories[iColor].size() << std::endl;
    }

    nlohmann::json j = categories;

    std::ofstream ofs(outputFile);
    ofs << j.dump();
}

GraphColoring::GraphClustering::GraphClustering(const std::vector<vector<int>>& inGraph, const std::vector<vector<float>>& inGraphWeights)
{
    graph = inGraph;
    graph_weights = inGraphWeights;
    for (size_t i = 0; i < graph.size(); i++)
    {
        graph_colors.push_back(-1);
        orgGraphNodeClusters.push_back(-1);
    }
}

void GraphColoring::GraphClustering::cluster(int Ks)
{
    while (Ks >= 1) {
        int u0 = minimumUnclusteredWeightNode(); // equation 16 from the paper
        if (u0 != -1) {

            vector<bool> visited;
            visited.resize(graph.size(), false);

            std::queue<int> q;

            visited[u0] = true;
            q.push(u0);

            while (!q.empty()) {
                int j = q.front();

                if (orgGraphNodeClusters[j] < 0) {
                    vector<int> constraintsR;
                    int r = clusteredGraph.size();

                    constraintsR.push_back(j);
                    orgGraphNodeClusters[j] = r;

                    //vector V is there to keep track of neighbor indices, since i need to sort graph[j] based on the values of graph_weights[j] 
                    vector<int> V(graph[j].size());
                    std::iota(V.begin(), V.end(), 0);
                    sort(V.begin(), V.end(), [&](int a, int b) {return graph_weights[j][a] < graph_weights[j][b]; });

                    for (int i = 0; i < V.size(); i++) {
                        // V[i] is the index of the next-smallest neighbor edge of node j
                        int cur = graph[j][V[i]];
                        if (orgGraphNodeClusters[cur] < 0 && constraintsR.size() < Ks) {
                            constraintsR.push_back(cur);
                            orgGraphNodeClusters[cur] = r;
                        }
                    }

                    if (constraintsR.size() == Ks) {
                        vector<int> newClusterVector;
                        clusteredGraph.push_back(newClusterVector);
                    }
                    else { // cluster is too small, undo
                        for (int i = 0; i < constraintsR.size(); i++) {
                            orgGraphNodeClusters[constraintsR[i]] = -1;
                        }
                    }
                }

                q.pop();

                //the rest of the BFS algorithm to update the queue
                for (int k = 0; k < graph[j].size(); k++) {
                    if (!visited[graph[j][k]]) {
                        visited[graph[j][k]] = true;
                        q.push(graph[j][k]);
                    }
                }
            }
        }

        Ks -= 1; // Try again with smaller clusters
    }


    // Calculating Cluster Graph Topology f
    // rom Vertices and Original Graph
    // Equation 14 from the paper
    clusteredGraphNodes.resize(clusteredGraph.size());
    for (int i = 0; i < graph.size(); i++) {
        int nodeCluster = orgGraphNodeClusters[i];
        clusteredGraphNodes[nodeCluster].push_back(i);

        for (int j = 0; j < graph[i].size(); j++) {
            int k = graph[i][j];

            int clusterI = orgGraphNodeClusters[i];
            int clusterK = orgGraphNodeClusters[k];
            if (clusterI != clusterK) {
                //making sure cluster edges are only added once per edge
                if (std::count(clusteredGraph[clusterI].begin(), clusteredGraph[clusterI].end(), orgGraphNodeClusters[k]) == 0) {
                    clusteredGraph[clusterI].push_back(orgGraphNodeClusters[k]);
                }
                if (std::count(clusteredGraph[clusterK].begin(), clusteredGraph[clusterK].end(), orgGraphNodeClusters[i]) == 0) {
                    clusteredGraph[clusterK].push_back(orgGraphNodeClusters[i]);
                }
            }
        }
    }
}

int GraphColoring::GraphClustering::minimumUnclusteredWeightNode()
{
    int u0 = -1;
    float minWeight = 1e30;
    for (int i = 0; i < graph.size(); i++) {
        if (orgGraphNodeClusters[i] < 0) {
            float curWeightSum = 0.0;
            for (int j = 0; j < graph[i].size(); j++) {
                curWeightSum += graph_weights[i][j];
            }

            if (curWeightSum < minWeight) {
                minWeight = curWeightSum;
                u0 = i;
            }
        }
    }

    return u0;
}

void GraphColoring::GraphClustering::saveClusteredColoringCategories(int numColors, 
    vector<int>& clusteredGraphColors, std::string outputFile)
{
    // nColors x nClusters x clusterSize
    vector<vector<vector<int>>> coloredClusterCategories(numColors);
    for (int iV = 0; iV < clusteredGraphColors.size(); iV++) {
        int color = clusteredGraphColors[iV];
        // in this 
        coloredClusterCategories[color].push_back(clusteredGraphNodes[iV]);
    }

    for (size_t iColor = 0; iColor < this->get_num_colors(); iColor++)
    {
        cout << "num nodes in colored category " << iColor << ": " << coloredClusterCategories[iColor].size() << std::endl;
    }

    nlohmann::json j = coloredClusterCategories;

    std::ofstream ofs(outputFile);
    ofs << j.dump();
}