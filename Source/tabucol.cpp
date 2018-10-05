
#include <queue>
#include <cstdlib>
#include <iostream>

#include "../Header/tabucol.hpp"

using std::queue;
using std::cerr;
using std::endl;

using std::cout;

GraphColoring::Tabucol::Tabucol(map<string, vector<string> > graph, int condition, int tabu_size, int rep, int nbmax) : GraphColor(graph) {
    this->condition = condition;
    this->tabu_size = tabu_size;
    this->rep = rep;
    this->nbmax = nbmax;
}

int GraphColoring::Tabucol::f(map<string,int> graph_colors) {
	int sum = 0;
	for(map< string,vector<string> >::iterator i = this->graph.begin(); i != this->graph.end(); i++) {
		for(unsigned j = 0; j< i->second.size(); j++) {
			if(this->graph_colors[i->first] == this->graph_colors[i->second[j]]) {
				sum += 1;
			}
		}
	}
	return sum;
}

map<string,int> GraphColoring::Tabucol::color() {
    if(this->condition <= 0) {
        return map<string,int>();
    }
	srand(time(NULL));
	for(map<string,vector<string>>::iterator i = this->graph.begin(); i != this->graph.end(); i++) {
		this->graph_colors[(*i).first] = rand() % this->condition;
	}

	queue<int> tabu_color;
	queue<string> tabu_vertex;
	for(int i = 0; i < this->tabu_size; i++) {
		map<string,vector<string>>::iterator x = this->graph.begin();
		std::advance(x,(rand() % this->graph.size()));
		tabu_vertex.push(x->first);
		tabu_color.push(rand() % this->condition);
	}

	int nbiter = 0;
	while(f(this->graph_colors) > 0 && nbiter < this->nbmax) {
		int best_color = -1;
		string best_vertex;
		int x = 0;
		int original_f = f(this->graph_colors);
		while(x < this->rep) {
			int flag = 0;
			int move_color;
			string move_vertex;
			while(!flag) {
				move_color = rand() % this->condition;
				map<string,vector<string>>::iterator mv = this->graph.begin();
				std::advance(mv,(rand() % this->graph.size()));
				move_vertex = mv->first;
				int inner_flag = 0;
				for(unsigned i = 0; i < tabu_vertex.size(); i++) {
					const string& temp_vertex = tabu_vertex.front();
					int temp_color = tabu_color.front();
					if(temp_vertex == move_vertex && temp_color == move_color) {
						flag = 1;
					}
					tabu_vertex.pop();
					tabu_color.pop();
					tabu_vertex.push(temp_vertex);
					tabu_color.push(temp_color);
				}
				if(!inner_flag) {
					flag = 1;
				}
			}
			if(best_color == -1) {
				best_color = move_color;
				best_vertex = move_vertex;
			}
			map<string,int> colors_move = this->graph_colors;
			colors_move[move_vertex] = move_color;
			map<string,int> colors_best = this->graph_colors;
			colors_best[best_vertex] = best_color;
			if(f(colors_move) < f(colors_best)) {
				best_vertex = move_vertex;
				best_color = move_color;
			}
			x += 1;
			if(f(colors_move) < original_f) {
				x = this->rep;
			}
		}
		if(best_color == -1) {
			cerr << "Best Color was never updated in the loop" << endl;
			this->graph_colors = map<string,int>();
			return this->graph_colors;
		}
		tabu_color.pop();
		tabu_color.push(best_color);
		tabu_vertex.pop();
		tabu_vertex.push(best_vertex);
		this->graph_colors[best_vertex] = best_color;
		nbiter += 1;
	}

	if(!this->is_valid()) {
		this->graph_colors = map<string,int>();
        return graph_colors;
	}

	return graph_colors;
}

