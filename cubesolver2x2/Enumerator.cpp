#include "Enumerator.h"
#include "Solver.h"
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
using std::vector;
using std::unordered_map;
using std::queue;
using std::cout;

struct State {
	Cube cube;
	int amountOfMoves = 0;
	int lastMoveType = 0;
};

void Enumerator::enumerateStates() {
	auto totalStart = Clock::now();
	
	cout << "Enumerating!\nInitializing Array... ";
	//unordered_map<int, uint8_t> minMoves;
	int8_t * minMoves;
	minMoves = new int8_t[3674160];
	for (int i = 0; i < 3674160; ++i) {
		minMoves[i] = 25;
	}
	cout << "Done (" << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - totalStart).count() / 1000000000 <<  ")\nStarting search...\n";
	int visited = 0;
	int lastAmountPrinted = -1;
	auto searchStart = Clock::now();
	queue<State> states;
	State start;
	start.cube = Cube();
	start.amountOfMoves = 0;
	start.lastMoveType = 3;
	states.push(start);
	State current;
	while (!states.empty()) {
		current = states.front();
		int index = Enumerator::indexCube(current.cube);

		if (minMoves[index] != 25) {
			states.pop();
			continue;
		}

		if (current.amountOfMoves == 11) {
			Cube::print(current.cube);
			cout << "index is " << index << '\n';
		}

		++visited;
		if (visited % 10000 == 0) {
			cout << "Visited " << visited << " unique states, " << states.size() << " cubes in queue\n";
		}
		minMoves[index] = current.amountOfMoves;

		for (int i = 0; i < 3; ++i) {
			if (current.lastMoveType == i) {
				continue;
			}

			for (int j = 0; j < 3; ++j) {
				State n;
				n.cube = Cube(current.cube);
				n.cube.move(i * 3 + j);
				n.amountOfMoves = current.amountOfMoves + 1;
				n.lastMoveType = i;
				//if (minMoves[Enumerator::indexCube(n.cube)] == 25) {
				states.push(n);
				//}
			}
		}

		states.pop();
	}

	cout << "Finished enumerating states\nVisited " << visited << " unique states\nTook ";
	cout << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - searchStart).count() / 1000000000 << " seconds\nCounting frequencies... ";
	auto countStart = Clock::now();
	int frequencies[100];
	for (int i = 0; i <= 30; ++i) {
		frequencies[i] = 0;
	}

	for (int i = 0; i < 3674160; ++i) {
		int x = minMoves[i];
		if (x != -1) {
			frequencies[x]++;
		}
	}
	cout << "Done(" << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - countStart).count() / 1000000000 << ")\nWriting to file... ";
	auto writeStart = Clock::now();
	std::ofstream output;
	output.open("corners.bin", std::ios::out | std::ios::binary);
	output.write((char*)minMoves, 3674160);
	output.close();
	cout << "Done (" << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - writeStart).count() / 1000000000 << ")\n";

	cout << "Storing frequencies:\n";
	output.open("frequencies.txt");
	int total = 0;
	for (int i = 0; i <= 30; ++i) {
		cout << "There are " << frequencies[i] << " positions that require " << i << " moves to solve\n";
		output << i << ' ' << frequencies[i] << '\n';
		total += frequencies[i];
	}
	cout << "total frequencies: " << total << '\n';
	output.close();

	cout << "Took " << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - totalStart).count() / 1000000000 << " seconds in total\n";
}

int Enumerator::indexCube(Cube c) {
	int total = 3674160;
	int n[7] = { 0, 1, 2, 3, 4, 5, 7, };
	vector<int> nums(n, n + 7);
	int index = 0;
	int divider = 7;
	int k;
	for (int i = 0; i < 6; ++i) {
		//find index
		k = 0;
		while (nums.at(k) != c.corners[i].identity) {
			++k;
		}
		total /= divider;
		index += k * total;
		total /= 3;
		index += c.corners[i].orientation * total;

		--divider;
		nums.erase(nums.begin() + k);
	}

	return index;
}