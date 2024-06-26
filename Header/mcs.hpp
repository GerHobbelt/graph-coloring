#ifndef _MCS_HPP_
#define _MCS_HPP_

#include "coloring_algorithm.hpp"

using GraphColoring::GraphColor;

namespace GraphColoring{
	class Mcs : public GraphColor {
		public: 
			/* Constructors */
			Mcs(map<string,vector<string>> graph) : GraphColor(graph) {} 

			/* Mutators */
			map<string,int> color();

			/* Accessors */
			string get_algorithm() { return "MCS"; }
	};

	class Mcs2 : public GraphColor2 {
	public:
		/* Constructors */
		Mcs2(vector<vector<int>> & graph) : GraphColor2(graph) {}

		/* Mutators */
		vector<int>& color();

		/* Accessors */
		string get_algorithm() { return "MCS"; }
	};
}

#endif //_MCS_HPP_
