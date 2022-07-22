#include "Cube.h"
#include "Solver.h"
#include <windows.h>
using std::vector;
using std::cout;


void setup() {
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(hConsole, &info);

	info.ColorTable[1] = RGB(255, 255, 255);
	info.ColorTable[2] = RGB(0, 255, 0);
	info.ColorTable[3] = RGB(255, 0, 0);
	info.ColorTable[4] = RGB(0, 0, 255);
	info.ColorTable[5] = RGB(255, 120, 30);
	info.ColorTable[6] = RGB(255, 255, 0);
	SetConsoleScreenBufferInfoEx(hConsole, &info);
}


int main() {
	setup();

	Cube cube = Cube();
	Solver solver = Solver(cube);
	
	std::string move;
	while (true) {
		Cube::print(solver.cube);
		std::cin >> move;
		if (move == "sc") {
			solver.scramble();
			continue;
		}
		else if (move == "s") {
			solver.iterativeDeepening();
			continue;
		}
		else if (move == "re") {
			cout << "reset!\n";
			Cube n = Cube();
			solver.cube = n;
		}
		else if (move == "cc") {
			Cube::createCubeFromFaces();
		}
		Cube::moveFromString(solver.cube, move);
		
	}
	return 0;
}