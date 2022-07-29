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
int rotationOrders[24][8] = {
	{0, 1, 2, 3, 4, 5, 6, 7,},
	{0, 2, 6, 4, 7, 5, 1, 3,},
	{0, 6, 1, 7, 3, 5, 2, 4,},
	
	{1, 3, 0, 2, 6, 4, 7, 5},
	{1, 0, 7, 6, 5, 4, 3, 2},
	{1, 7, 3, 5, 2, 4, 0, 6},

	{2, 0, 3, 1, 5, 7, 4, 6},
	{2, 3, 4, 5, 6, 7, 0, 1},
	{2, 4, 0, 6, 1, 7, 3, 5},

	{3, 2, 1, 0, 7, 6, 5, 4},
	{3, 1, 5, 7, 4, 6, 2, 0},
	{3, 5, 2, 4, 0, 6, 1, 7},

	{4, 5, 6, 7, 0, 1, 2, 3},
	{4, 6, 2, 0, 3, 1, 5, 7},
	{4, 2, 5, 3, 7, 1, 6, 0},

	{5, 7, 4, 6, 2, 0, 3, 1},
	{5, 4, 3, 2, 1, 0, 7, 6},
	{5, 3, 7, 1, 6,0, 4, 2},

	{6, 4, 7, 5, 1, 3, 0, 2},
	{6, 7, 0, 1, 2, 3, 4, 5},
	{6, 0, 4, 2, 5, 3, 7, 1},
	
	{7, 6, 5, 4, 3, 2, 1, 0},
	{7, 5, 1, 3, 0, 2, 6, 4},
	{7, 1, 6, 0, 4, 2, 5, 3}
};
int largestIndex = -1;
bool used[28784];

void printTime(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
	cout << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000000000;
}

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

int getLowestIndex(vector<int> perm) {
	//cout << "called\n";
	//cout << "\nperm is\n";
	/*for (int i = 0; i < 8; ++i) {
		cout << perm.at(i) << ' ';
	}
	cout << '\n';*/

	int lowestIndex = 2147483647;
	for (int j = 0; j < 24; ++j) {
		int total = 40320;
		vector<int> nums(rotationOrders[j], rotationOrders[j] + 8);
		
		//for (int i = 0; i < 8; ++i) {
		//	cout << nums.at(i) << ' ';
		//}
		
		
		
		int index = 0;
		int divider = 8;
		int k;
		for (int i = 0; i < 7; ++i) {
			//find index
			k = 0;
			while (nums.at(k) != perm.at(rotationOrders[j][i])) {
				++k;
			}
			total /= divider;
			index += k * total;
			//cout << total << "t ";

			--divider;
			nums.erase(nums.begin() + k);
		}
		//cout << index << '\n';

		if (index < lowestIndex) {
			lowestIndex = index;
		}
	}
	//cout << "exited\n";
	//int x;
	//std::cin >> x;
	return lowestIndex;
}

void evalState() {

}

void genState(vector<int> path, vector<int> left) {
	if (left.size() == 0) {
		int ans = getLowestIndex(path);
		used[ans] = true;
		if (ans > largestIndex) {
			largestIndex = ans;
		}
		return;
	}

	if (path.size() == 1) {
		cout << "Trying path with the first number as " << path.at(0) << '\n';
	}

	//if (path.size() == 2 && path.at(1) != 7) {
	//	return;
	//}

	//if (path.size() == 3 &&  path.at(2) != 4 ) {
	//	return;
	//}

	//if (path.size() == 4 && path.at(3) != 2) {
	//	return;
	//}

	//if (path.size() == 5 && path.at(4) != 5) {
	//	return;
	//}

	//if (path.size() == 6 && path.at(5) != 6) {
	//	return;
	//}

	//if (path.size() == 7 && path.at(6) != 0) {
	//	return;
	//}

	for (int i = 0; i < left.size(); ++i) {
		int doing = left.at(i);
		path.push_back(doing);
		left.erase(left.begin() + i);
		genState(path, left);
		left.insert(left.begin() + i, doing);
		path.pop_back();
	}
}

int Enumerator::bruteforceLowestIndexNum() {
	vector<int> allNums = { 0, 1, 2, 3, 4, 5, 6, 7 };
	vector<int> other = { 1, 7, 4, 2, 5, 6, 0, 3 };
	//getLowestIndex(other);
	//getLowestIndex(allNums);
	
	vector<int> empty;
	for (int i = 0; i <= 4488; ++i) {
		used[i] = false;
	}
	cout << "Starting...\n";
	auto start = Clock::now();
	genState(empty, allNums);
	auto end = Clock::now();
	cout << "Done looking through states! Took ";
	printTime(start, end);
	cout << " seconds\nLowest index using only rotations is " << largestIndex;
	int count = 0;
	for (int i = 0; i <= 4488; ++i) {
		if (!used[i]) {
			++count;
		}
	}
	cout << "\nThere are " << count << " unused slots!\n";
	std::ofstream out;
	out.open("rotationLowestIndex");
	out << largestIndex;
	out.close();
	
	
	return 0;
}