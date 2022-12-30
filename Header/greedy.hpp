#ifndef _GREEDY_HPP_
#define _GREEDY_HPP_

#include "coloring_algorithm.hpp"


namespace GraphColoring {

	class OrderedGreedy : public GraphColor2 {
	public:
		/* Constructors */
		OrderedGreedy(vector<vector<int>>& graph) : GraphColor2(graph) {}

		virtual int nextNode();
		void reduceDegree(int iNode);
		/* Mutators */
		vector<int>& color();

		/* Accessors */
		string get_algorithm() { return "MCS"; }

		std::vector<int> degrees;
		std::vector<bool> colored;
	};
}

#endif //_MCS_HPP_