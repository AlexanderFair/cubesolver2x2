#include "Solver.h"
#include "Enumerator.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
typedef std::chrono::high_resolution_clock Clock;
using std::cout;
using std::vector;


Solver::Solver(Cube c) {
	cube = c;
}

void Solver::scramble() {
	unsigned int t = (unsigned int) time(nullptr);
	std::srand(t);
	cout << "Enter how many moves to scramble\n";
	int moves;
	std::cin >> moves;
	cout << "Scramble is: ";
	//first move
	int lastMove = (int)((float)std::rand() / RAND_MAX * 3);
	int change = (int)((float)std::rand() / RAND_MAX * 3);
	cube.move(lastMove * 3 + change);
	cout << Cube::getMoveInStrForm(lastMove * 3 + change) << ' ';
	moves--;
	while (moves > 0) {
		int newMove = (int)((float) std::rand() / RAND_MAX * 3);
		if (newMove == lastMove) {
			newMove += 1 + (int)((float)std::rand() / RAND_MAX * 2);
			newMove %= 3;
		}
		change = (int)((float)std::rand() / RAND_MAX * 3);
		cube.move(newMove * 3 + change);
		lastMove = newMove;
		cout << Cube::getMoveInStrForm(newMove * 3 + change) << ' ';
		moves--;
	}
	Cube::print(cube);
}

vector<int> Solver::dfs(vector<int> path, int movesLeft) {
	//cout << movesLeft << '\n';
	//Cube::print(cube);
	if (movesLeft == 0) {
		if (Cube::isSolved(cube)) {
			return path;
		}
		else {
			vector<int> empty;
			return empty;
		}
	}

	int lastMove = path.back() / 3;

	for (int i = 0; i < 3; i++) {
		
		if (i == lastMove) {
			continue;
		}

		for (int j = 0; j < 3; j++) {
			int thisMove = i * 3 + j;
			//cout << "\tDoing move " << Cube::getMoveInStrForm(thisMove) << '\n';
			path.push_back(thisMove);
			cube.move(thisMove);
			vector<int> returned = dfs(path, movesLeft - 1);
			if (!returned.empty()) {
				return returned;
			}

			cube.unmove(thisMove);
			path.pop_back();
		}
	}



	vector<int> empty;
	return empty;
}

void Solver::iterativeDeepening() {
	if (Cube::isSolved(cube)) {
		cout << "Already solved!\n";
		return;
	}

	cout << "Enter the max depth that it will search\n";
	int maxDepth;
	std::cin >> maxDepth;
	auto totalStart = Clock::now();
	vector<int> r;
	for (int d = 1; d <= maxDepth; d++) {
		auto iterStart = Clock::now();
		vector<int> e;
		for (int i = 0; i < 9; i++) {
			e.push_back(i);
			cube.move(i);
			//cout << "Doing " << Cube::getMoveInStrForm(i) << " move\n";
			r = dfs(e, d - 1);
			
			if (!r.empty()) {
				break;
			}

			cube.unmove(i);
			e.pop_back();
		}
		if (!r.empty()) {
			break;
		}
		cout << "Couldn't find a solution with a depth of " << d;
		cout << " (took " << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - iterStart).count() / 1000000000 << " seconds)";
		cout << ", increasing depth...\n";
	}

	if (r.empty()) {
		cout << "Couldn't find a solution :(\n";
		return;
	}

	cout << "Found a solution! It is:\n";
	for (int i = 0; i < r.size(); i++) {
		cout << Cube::getMoveInStrForm(r.at(i)) << ' ';
	}
	cout << "\nTotal time taken: " << (float) std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - totalStart).count() / 1000000000 << " seconds\n";

}

void Solver::solve() {
	vector<int> path = excatHeuristic();
	if (path.empty()) {
		cout << "Already solved!\n";
	}
	else if (path.at(0) == -1) {
		cout << "Couldn't find a solution :(\n";
	}
	else {
		cout << "Solution:\n";
		for (int i = 0; i < path.size(); ++i) {
			cout << Cube::getMoveInStrForm(path.at(i)) << ' ';
		}
		cout << "(" << path.size() << " moves)\n";
	}
}

vector<int> Solver::excatHeuristic() {
	auto start = Clock::now();
	cout << "Loading files...";
	uint8_t* minMoves;
	minMoves = new uint8_t[3674160];
	std::ifstream file("corners.bin", std::ios::in | std::ios::binary);
	file.read((char*)minMoves, 3674160);
	file.close();
	cout << " Done (" << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - start).count() / 1000000000 << ")\nFinding Solution...";

	vector<int> path;
	int runs = 0;
	while (runs < 100) {
		++runs;
		if (Cube::isSolved(cube)) {
			cout << " Done (" << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - start).count() / 1000000000 << ")\n";
			return path;
		}
		//expand cube
		int lowestMoveAmount = 100;
		int besti = 9;
		for (int i = 0; i < 9; ++i) {
			cube.move(i);
			int index = Enumerator::indexCube(cube);
			if (minMoves[index] == 11) {
				Cube::print(cube);
			}
			if (minMoves[index] < lowestMoveAmount) {
				besti = i;
				lowestMoveAmount = minMoves[index];
			}
			cube.unmove(i);
		}

		path.push_back(besti);
		cube.move(besti);
	}

	path.clear();
	path.push_back(-1);
	return path;
}

struct State {
	Cube cube;
	vector<uint8_t> path;
	uint8_t lastMove : 2;
};

void Solver::breadthFirst() {
	vector<State> states;
	State current;
	current.cube = Cube(cube);
	current.lastMove = 3;
	states.push_back(current);
	

	vector<uint8_t> solution;
	
	while (!states.empty()) {
		State current = states.front();
		if (Cube::isSolved(current.cube)) {
			solution = current.path;
			break;
		}

		for (int i = 0; i < 3; i++) {
			if (i == current.lastMove) {
				continue;
			}

			for (int j = 0; j < 3; j++) {
				int thisMove = i * 3 + j;
				State news;
				news.lastMove = i;
				news.path = current.path;
				news.path.push_back(thisMove);
				news.cube = Cube(current.cube);
				news.cube.move(thisMove);
				states.push_back(news);
			}
		}
		states.erase(states.begin());
	}

	//print solution
	if (solution.empty()) {
		cout << "Didn't find a solution :(\n";
		return;
	}

	cout << "Solution:\n";
	for (int i = 0; i < solution.size(); i++) {
		cout << Cube::getMoveInStrForm(solution.at(i)) << " ";
	}
	cout << '\n';

}