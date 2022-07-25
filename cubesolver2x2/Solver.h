#pragma once
#include "Cube.h"
#include <vector>

class Solver
{
public:
	Solver(Cube c);
	void iterativeDeepening();
	void breadthFirst();
	std::vector<int> excatHeuristic();
	void scramble();
	void solve();
	Cube cube;
private:
	std::vector<int> dfs(std::vector<int> path, int movesLeft);
};