#pragma once
#include "Cube.h"
#include <vector>

class Solver
{
public:
	Solver(Cube c);
	void iterativeDeepening();
	void breadthFirst();
	void scramble();
	Cube cube;
private:
	std::vector<int> dfs(std::vector<int> path, int movesLeft);
};