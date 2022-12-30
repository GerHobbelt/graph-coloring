
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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

}

void GraphColoring::GraphColor2::saveColoringCategories(std::string outputFile)
{
    size_t numColors = get_num_colors();
    vector<vector<int>> categories(numColors);

    for (int iV = 0; iV < size(); iV++) {
        int color = get_color(iV);
        // in this 
        categories[color].push_back(iV);
    }

    for (size_t iColor = 0; iColor < this->get_num_colors(); iColor++)
    {
        cout << "num nodes in colored category " << iColor << ": " << categories[iColor].size() << std::endl;
    }

    nlohmann::json j = categories;

    std::ofstream ofs(outputFile);
    ofs << j.dump();
}