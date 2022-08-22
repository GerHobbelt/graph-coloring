#ifndef _GRAPH_COLOR_HPP_
#define _GRAPH_COLOR_HPP_

#include <map>
#include <string>
#include <vector>

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
        void saveColoringCategories(std::string outputFile);

    };
}

#endif // _GRAPH_COLOR_HPP_
