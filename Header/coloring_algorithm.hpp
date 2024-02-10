#ifndef _GRAPH_COLOR_HPP_
#define _GRAPH_COLOR_HPP_

#include <map>
#include <string>
#include <vector>
#include <iostream>

using std::map;
using std::string;
using std::vector;

namespace GraphColoring {
    class GraphColor {
        protected:
            /* Internal Members */
            map<string,vector<string>> graph;
            map<string,int> graph_colors;

        public:
            /* Constructors */
            GraphColor();
            GraphColor(map<string, vector<string>> graph);

            /* Mutators */
            virtual map<string,int> color() = 0;
            void set_graph(map<string,vector<string>> new_graph) { this->graph = new_graph; }
            void modify_graph(string node, vector<string> neighbors) { this->graph[node] = neighbors; }

            /* Accessors */
            virtual string get_algorithm() = 0;
            unsigned size() { return this->graph.size(); }
            bool is_colored();
            map<string,int> get_coloring() { return this->graph_colors; }
            int get_color(string node);
            int get_num_colors();
            bool is_valid();

            /* Print functions */
            void print_coloring();
            void print_chromatic();
            void saveColoringCategories(std::string outputFile);

    };

    class GraphColor2 {
    protected:
        /* Internal Members */
        vector<vector<int>> graph; // node x neighbors
        vector<int> graph_colors;

    public:
        /* Constructors */
        GraphColor2();
        GraphColor2(const std::vector<vector<int>>& graph);

        /* Mutators */
        virtual vector<int> & color() = 0;
        void set_graph(const vector<vector<int>> & new_graph) { this->graph = new_graph; }
        void modify_graph(int node, const vector<int> & neighbors) { this->graph[node] = neighbors; }

        /* Accessors */
        virtual string get_algorithm() = 0;
        unsigned size() { return this->graph.size(); }
        bool is_colored();
        vector<int>& get_coloring() { return this->graph_colors; }
        int get_color(int node);
        int get_num_colors();
        bool is_valid();

        /* Print functions */
        //void print_coloring();
        void print_chromatic();
        void convertToColoredCategories();
        float findLargestSmallestCategories(int& biggestCategory, int& smallestCategory);
        // return the category id of the changable node, not the node id, -1 if not changable 
        int findChangableNodeInCategory(int sourceColor, int destinationColor);
        void changeColor(int sourceColor, int categoryId, int destinationColor);
        bool changable(int node, int destinationColor);
        void balanceColoredCategories(float minMaxRatio=1.5);
        void saveColoringCategories(std::string outputFile);

        vector<vector<int>> categories;
        
    };

    class GraphClustering : public GraphColor2 {
    public:
        GraphClustering(const std::vector<vector<int>>& inGraph, const std::vector<vector<float>>& inGraphWeights);
        void cluster(int Ks);
        // 1 - sharedVerts / (8 - sharedVerts)
        vector<vector<float>> graph_weights;
        // graph_clusters[i] is the cluster that node i belongs to;
        // if graph_clusters[i] returns -1, the constraint has not been placed in a cluster yet;
        vector<int> orgGraphNodeClusters;
        // the new graph made of the clustered nodes
        vector<vector<int>> clusteredGraph;
        // the original nodes that each node of the clustered graph contains
        vector<vector<int>> clusteredGraphNodes;
        int minimumUnclusteredWeightNode();
        void saveClusteredColoringCategories(int numColors, vector<int>& clusteredGraphColors, std::string outputFile);

    private:
        virtual vector<int>& color() { std::cout << "This is a clustering graph, don't use it for coloring!\n"; return graph_colors; }
        virtual string get_algorithm() { return "clustering"; }

    };
}

#endif // _GRAPH_COLOR_HPP_
