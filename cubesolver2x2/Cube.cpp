#include "Cube.h"
#include <algorithm>
#include <windows.h>
using std::cout;

const int Cube::faceChanges[6][4] = {
		{1, 2, 3, 4},
		{0, 4, 5, 2},
		{0, 1, 5, 3},
		{0, 2, 5, 4},
		{0, 3, 5, 1},
		{1, 4, 3, 2},
};

Cube::Cube() {
	for (int i = 0; i < 8; i++) {
		corners[i] = Cuboid(i, 0);
	}
	frontFace = 1;
	topFace = 0;
}

void printChar(char c) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (c) {
	case 'w':
		SetConsoleTextAttribute(hConsole, 17);
		break;
	case 'g':
		SetConsoleTextAttribute(hConsole, 34);
		break;
	case 'r':
		SetConsoleTextAttribute(hConsole, 51);
		break;
	case 'b':
		SetConsoleTextAttribute(hConsole, 68);
		break;
	case 'o':
		SetConsoleTextAttribute(hConsole, 85);
		break;
	case 'y':
		SetConsoleTextAttribute(hConsole, 102);
		break;
	default:
		;
	}
	std::cout << c << ' ';
	SetConsoleTextAttribute(hConsole, 15);
}

void xRotateFaces(char faces[6][4]) {
	for (int i = 0; i < 3; i++) { // loop because i wrote this rotation in the wrong direction, so this is the bandaid fix.
		//it also doesn't *really* matter for speed reasons as this only gets used while printing, which does not need to be that efficent
		//left side
		char temp = faces[4][0];
		faces[4][0] = faces[4][2];
		faces[4][2] = faces[4][3];
		faces[4][3] = faces[4][1];
		faces[4][1] = temp;
		//right side
		temp = faces[2][0];
		faces[2][0] = faces[2][1];
		faces[2][1] = faces[2][3];
		faces[2][3] = faces[2][2];
		faces[2][2] = temp;
		//top left
		temp = faces[0][0];
		faces[0][0] = faces[3][3];
		faces[3][3] = faces[5][0];
		faces[5][0] = faces[1][0];
		faces[1][0] = temp;
		//top right
		temp = faces[0][1];
		faces[0][1] = faces[3][2];
		faces[3][2] = faces[5][1];
		faces[5][1] = faces[1][1];
		faces[1][1] = temp;
		//bottom left
		temp = faces[0][2];
		faces[0][2] = faces[3][1];
		faces[3][1] = faces[5][2];
		faces[5][2] = faces[1][2];
		faces[1][2] = temp;
		//bottom right
		temp = faces[0][3];
		faces[0][3] = faces[3][0];
		faces[3][0] = faces[5][3];
		faces[5][3] = faces[1][3];
		faces[1][3] = temp;
	}
}

void yRotateFaces(char faces[6][4]) {
	//top face
	char temp = faces[0][0];
	faces[0][0] = faces[0][2];
	faces[0][2] = faces[0][3];
	faces[0][3] = faces[0][1];
	faces[0][1] = temp;
	//bottom face
	temp = faces[5][0];
	faces[5][0] = faces[5][1];
	faces[5][1] = faces[5][3];
	faces[5][3] = faces[5][2];
	faces[5][2] = temp;
	//top left
	temp = faces[1][0];
	faces[1][0] = faces[2][0];
	faces[2][0] = faces[3][0];
	faces[3][0] = faces[4][0];
	faces[4][0] = temp;
	//top right
	temp = faces[1][1];
	faces[1][1] = faces[2][1];
	faces[2][1] = faces[3][1];
	faces[3][1] = faces[4][1];
	faces[4][1] = temp;
	//bottom left
	temp = faces[1][2];
	faces[1][2] = faces[2][2];
	faces[2][2] = faces[3][2];
	faces[3][2] = faces[4][2];
	faces[4][2] = temp;
	//bottom right
	temp = faces[1][3];
	faces[1][3] = faces[2][3];
	faces[2][3] = faces[3][3];
	faces[3][3] = faces[4][3];
	faces[4][3] = temp;
}

void zRotateFaces(char faces[6][4]) {
	//front side
	char temp = faces[1][0];
	faces[1][0] = faces[1][3];
	faces[1][3] = faces[1][2];
	faces[1][2] = faces[1][1];
	faces[1][1] = temp;
	//back side
	temp = faces[3][0];
	faces[3][0] = faces[3][1];
	faces[3][1] = faces[3][3];
	faces[3][3] = faces[3][2];
	faces[3][2] = temp;
	//top left
	temp = faces[0][0];
	faces[0][0] = faces[4][2];
	faces[4][2] = faces[5][3];
	faces[5][3] = faces[2][1];
	faces[2][1] = temp;
	//top right
	temp = faces[0][1];
	faces[0][1] = faces[4][0];
	faces[4][0] = faces[5][2];
	faces[5][2] = faces[2][3];
	faces[2][3] = temp;
	//bottom left
	temp = faces[0][2];
	faces[0][2] = faces[4][3];
	faces[4][3] = faces[5][1];
	faces[5][1] = faces[2][0];
	faces[2][0] = temp;
	//bottom right
	temp = faces[0][3];
	faces[0][3] = faces[4][1];
	faces[4][1] = faces[5][0];
	faces[5][0] = faces[2][2];
	faces[2][2] = temp;
}

void printFaces(char faces[6][4]) {
	//w 
	for (int i = 0; i < 6; i++) {
		std::cout << "                ";
		printChar(faces[0][2 * (i / 3) + 0]);
		printChar(faces[0][2 * (i / 3) + 0]);
		printChar(faces[0][2 * (i / 3) + 0]);
		printChar(faces[0][2 * (i / 3) + 1]);
		printChar(faces[0][2 * (i / 3) + 1]);
		printChar(faces[0][2 * (i / 3) + 1]);
		std::cout << std::endl;
	}
	std::cout << '\n';
	//middles
	int midf[4] = { 4, 1, 2, 3 };

	for (int i = 0; i < 6; i++) {
		for (int f = 0; f < 4; f++) {
			std::cout << "  ";
			printChar(faces[midf[f]][2 * (i / 3) + 0]);
			printChar(faces[midf[f]][2 * (i / 3) + 0]);
			printChar(faces[midf[f]][2 * (i / 3) + 0]);
			printChar(faces[midf[f]][2 * (i / 3) + 1]);
			printChar(faces[midf[f]][2 * (i / 3) + 1]);
			printChar(faces[midf[f]][2 * (i / 3) + 1]);
		}
		std::cout << std::endl;
	}
	std::cout << '\n';
	//y
	for (int i = 0; i < 6; i++) {
		std::cout << "                ";
		printChar(faces[5][2 * (i / 3) + 0]);
		printChar(faces[5][2 * (i / 3) + 0]);
		printChar(faces[5][2 * (i / 3) + 0]);
		printChar(faces[5][2 * (i / 3) + 1]);
		printChar(faces[5][2 * (i / 3) + 1]);
		printChar(faces[5][2 * (i / 3) + 1]);
		std::cout << std::endl;
	}
}

void Cube::print(Cube c) {
	std::cout << "\n\n\n";

	cout << "Move conversions: (input - output)\n";
	for (int i = 0; i < 18; i++) {
		cout << i << " - " << c.moveConversions[i] << '\n';
	}
	cout << "\nTop Face - " << c.topFace << "\nFront Face - " << c.frontFace << "\n\n";

	char colorById[8][3] = {
		{'w', 'b', 'o'},
		{'w', 'r', 'b'},
		{'w', 'o', 'g'},
		{'w', 'g', 'r'},
		{'y', 'g', 'o'},
		{'y', 'r', 'g'},
		{'y', 'o', 'b'},
		{'y', 'b', 'r'},
	};

	for (int i = 0; i < 8; i++) {
		std::cout << int(c.corners[i].identity) << ' ' << int(c.corners[i].orientation) << std::endl;
	}

	char faces[6][4];
	//w
	faces[0][0] = colorById[c.corners[0].identity][c.corners[0].orientation];
	faces[0][1] = colorById[c.corners[1].identity][c.corners[1].orientation];
	faces[0][2] = colorById[c.corners[2].identity][c.corners[2].orientation];
	faces[0][3] = colorById[c.corners[3].identity][c.corners[3].orientation];
	//y
	faces[5][0] = colorById[c.corners[4].identity][c.corners[4].orientation];
	faces[5][1] = colorById[c.corners[5].identity][c.corners[5].orientation];
	faces[5][2] = colorById[c.corners[6].identity][c.corners[6].orientation];
	faces[5][3] = colorById[c.corners[7].identity][c.corners[7].orientation];
	//g
	faces[1][0] = colorById[c.corners[2].identity][(c.corners[2].orientation + 2) % 3];
	faces[1][1] = colorById[c.corners[3].identity][(c.corners[3].orientation + 1) % 3];
	faces[1][2] = colorById[c.corners[4].identity][(c.corners[4].orientation + 1) % 3];
	faces[1][3] = colorById[c.corners[5].identity][(c.corners[5].orientation + 2) % 3];
	//r
	faces[2][0] = colorById[c.corners[3].identity][(c.corners[3].orientation + 2) % 3];
	faces[2][1] = colorById[c.corners[1].identity][(c.corners[1].orientation + 1) % 3];
	faces[2][2] = colorById[c.corners[5].identity][(c.corners[5].orientation + 1) % 3];
	faces[2][3] = colorById[c.corners[7].identity][(c.corners[7].orientation + 2) % 3];
	//b
	faces[3][0] = colorById[c.corners[1].identity][(c.corners[1].orientation + 2) % 3];
	faces[3][1] = colorById[c.corners[0].identity][(c.corners[0].orientation + 1) % 3];
	faces[3][2] = colorById[c.corners[7].identity][(c.corners[7].orientation + 1) % 3];
	faces[3][3] = colorById[c.corners[6].identity][(c.corners[6].orientation + 2) % 3];
	//o
	faces[4][0] = colorById[c.corners[0].identity][(c.corners[0].orientation + 2) % 3];
	faces[4][1] = colorById[c.corners[2].identity][(c.corners[2].orientation + 1) % 3];
	faces[4][2] = colorById[c.corners[6].identity][(c.corners[6].orientation + 1) % 3];
	faces[4][3] = colorById[c.corners[4].identity][(c.corners[4].orientation + 2) % 3];
	int cycle[4] = {0, 1, 5, 3};
	int tf = 0;
	int ff = 1;
	cout << "starting cycle\n";
	while (cycle[tf] != c.topFace && cycle[ff] != c.frontFace) {
		tf++;
		tf %= 4;
		ff++;
		ff %= 4;
		xRotateFaces(faces);
		cout << "x rotating once\n";
		//printFaces(faces);
	}
	cout << "done cycle\n";

	if (cycle[tf] == c.topFace) {
		ff = cycle[ff];
		while (ff != c.frontFace) {
			ff = getNextFrontFace(cycle[tf], ff);
			yRotateFaces(faces);
			cout << "y rotating once\n";
			//printFaces(faces);
		}
	}
	else {
		tf = cycle[tf];
		while (tf != c.topFace) {
			tf = getNextTopFace(tf, cycle[ff]);
			zRotateFaces(faces);
			cout << "z rotating once\n";
			//printFaces(faces);
		}
	}

	//print the faces	
	//w 
	for (int i = 0; i < 6; i++) {
		std::cout << "                ";
		printChar(faces[0][2 * (i / 3) + 0]);
		printChar(faces[0][2 * (i / 3) + 0]);
		printChar(faces[0][2 * (i / 3) + 0]);
		printChar(faces[0][2 * (i / 3) + 1]);
		printChar(faces[0][2 * (i / 3) + 1]);
		printChar(faces[0][2 * (i / 3) + 1]);
		std::cout << std::endl;
	}
	std::cout << '\n';
	//middles
	int midf[4] = { 4, 1, 2, 3 };

	for (int i = 0; i < 6; i++) {
		for (int f = 0; f < 4; f++) {
			std::cout << "  ";
			printChar(faces[midf[f]][2 * (i / 3) + 0]);
			printChar(faces[midf[f]][2 * (i / 3) + 0]);
			printChar(faces[midf[f]][2 * (i / 3) + 0]);
			printChar(faces[midf[f]][2 * (i / 3) + 1]);
			printChar(faces[midf[f]][2 * (i / 3) + 1]);
			printChar(faces[midf[f]][2 * (i / 3) + 1]);
		}
		std::cout << std::endl;
	}
	std::cout << '\n';
	//y
	for (int i = 0; i < 6; i++) {
		std::cout << "                ";
		printChar(faces[5][2 * (i / 3) + 0]);
		printChar(faces[5][2 * (i / 3) + 0]);
		printChar(faces[5][2 * (i / 3) + 0]);
		printChar(faces[5][2 * (i / 3) + 1]);
		printChar(faces[5][2 * (i / 3) + 1]);
		printChar(faces[5][2 * (i / 3) + 1]);
		std::cout << std::endl;
	} 

	cout << "\n\n\ncalling next front face with a bunch of things\n";
	cout << getNextFrontFace(5, 1) << ' ' << getNextFrontFace(5, 2) << ' ' << getNextFrontFace(5, 3);
	cout << ' ' << getNextFrontFace(5, 4) << '\n';

}

void Cube::moveFromString(Cube& c, std::string s) {
	int choice = 0;
	if (s == "u") choice = 0;
	else if (s == "u2") choice = 1;
	else if (s == "up") choice = 2;
	else if (s == "f") choice = 3;
	else if (s == "f2") choice = 4;
	else if (s == "fp") choice = 5;
	else if (s == "r") choice = 6;
	else if (s == "r2") choice = 7;
	else if (s == "rp") choice = 8;
	else if (s == "d") choice = 9;
	else if (s == "d2") choice = 10;
	else if (s == "dp") choice = 11;
	else if (s == "b") choice = 12;
	else if (s == "b2") choice = 13;
	else if (s == "bp") choice = 14;
	else if (s == "l") choice = 15;
	else if (s == "l2") choice = 16;
	else if (s == "lp") choice = 17;
	else {
		std::cout << "Illegal Move\n";
		return;
	}
	cout << "move was " << choice << '\n';
	choice = c.moveConversions[choice];
	cout << "move now is " << choice << '\n';
	c.move(choice);
}

void Cube::move(int move) {
	switch (move) {
	case 0:
		umove();
		break;
	case 1:
		u2move();
		break;
	case 2:
		uprime();
		break;
	case 3:
		fmove();
		break;
	case 4:
		f2move();
		break;
	case 5:
		fprime();
		break;
	case 6:
		rmove();
		break;
	case 7:
		r2move();
		break;
	case 8:
		rprime();
		break;
	case 9://d
		umove();
		ymove(*this);
		ymove(*this);
		ymove(*this);
		break;
	case 10://d2
		u2move();
		ymove(*this);
		ymove(*this);
		break;
	case 11://dp
		uprime();
		ymove(*this);
		break;
	case 12://b
		fmove();
		zmove(*this);
		zmove(*this);
		zmove(*this);
		break;
	case 13://b2
		f2move();
		zmove(*this);
		zmove(*this);
		break;
	case 14://bp
		fprime();
		zmove(*this);
		break;
	case 15://l
		rmove();
		xmove(*this);
		xmove(*this);
		xmove(*this);
		break;
	case 16://l2
		r2move();
		xmove(*this);
		xmove(*this);
		break;
	case 17://lp
		rprime();
		xmove(*this);
		break;
	default:
		std::cout << "Illegal move" << std::endl;
	}
}


void Cube::rmove() {
	corners[1].add(2);
	corners[3].add(1);
	corners[5].add(2);
	corners[7].add(1);

	Cuboid temp = corners[1];
	corners[1] = corners[3];
	corners[3] = corners[5];
	corners[5] = corners[7];
	corners[7] = temp;
}
void Cube::r2move(){
	std::swap(corners[1], corners[5]);
	std::swap(corners[3], corners[7]);
}
void Cube::rprime() {
	corners[1].add(2);
	corners[3].add(1);
	corners[5].add(2);
	corners[7].add(1);

	Cuboid temp = corners[1];
	corners[1] = corners[7];
	corners[7] = corners[5];
	corners[5] = corners[3];
	corners[3] = temp;
}

void Cube::umove() {
	Cuboid temp = corners[0];
	corners[0] = corners[2];
	corners[2] = corners[3];
	corners[3] = corners[1];
	corners[1] = temp;
}
void Cube::u2move() {
	std::swap(corners[0], corners[3]);
	std::swap(corners[1], corners[2]);
}
void Cube::uprime() {
	Cuboid temp = corners[0];
	corners[0] = corners[1];
	corners[1] = corners[3];
	corners[3] = corners[2];
	corners[2] = temp;
}

void Cube::fmove() {
	corners[3].add(2);
	corners[2].add(1);
	corners[4].add(2);
	corners[5].add(1);

	Cuboid temp = corners[2];
	corners[2] = corners[4];
	corners[4] = corners[5];
	corners[5] = corners[3];
	corners[3] = temp;
}
void Cube::f2move() {
	std::swap(corners[2], corners[5]);
	std::swap(corners[3], corners[4]);
}
void Cube::fprime() {
	corners[3].add(2);
	corners[2].add(1);
	corners[4].add(2);
	corners[5].add(1);

	Cuboid temp = corners[2];
	corners[2] = corners[3];
	corners[3] = corners[5];
	corners[5] = corners[4];
	corners[4] = temp;
}


void Cube::xmove(Cube& c) {
	int temp = c.moveConversions[3];
	c.moveConversions[3] = c.moveConversions[9];
	c.moveConversions[9] = c.moveConversions[12];
	c.moveConversions[12] = c.moveConversions[0];
	c.moveConversions[0] = temp;

	temp = c.moveConversions[4];
	c.moveConversions[4] = c.moveConversions[10];
	c.moveConversions[10] = c.moveConversions[13];
	c.moveConversions[13] = c.moveConversions[1];
	c.moveConversions[1] = temp;

	temp = c.moveConversions[5];
	c.moveConversions[5] = c.moveConversions[11];
	c.moveConversions[11] = c.moveConversions[14];
	c.moveConversions[14] = c.moveConversions[2];
	c.moveConversions[2] = temp;

	cout << "in xmove\n";
	int currentTop = c.topFace;
	c.topFace = getNextTopFace(c.topFace, c.frontFace);
	int nextFront = getNextTopFace(c.topFace, c.frontFace);
	c.topFace = c.frontFace;
	c.frontFace = nextFront;
}
void Cube::ymove(Cube& c) {
	//f now means r
	int temp = c.moveConversions[3];
	c.moveConversions[3] =c. moveConversions[6];
	c.moveConversions[6] = c.moveConversions[12];
	c.moveConversions[12] = c.moveConversions[15];
	c.moveConversions[15] = temp;

	//f2 - r2
	temp = c.moveConversions[4];
	c.moveConversions[4] = c.moveConversions[7];
	c.moveConversions[7] = c.moveConversions[13];
	c.moveConversions[13] = c.moveConversions[16];
	c.moveConversions[16] = temp;

	//fp - rp
	temp = c.moveConversions[5];
	c.moveConversions[5] = c.moveConversions[8];
	c.moveConversions[8] = c.moveConversions[14];
	c.moveConversions[14] = c.moveConversions[17];
	c.moveConversions[17] = temp;

	c.frontFace = getNextFrontFace(c.topFace, c.frontFace);
}
void Cube::zmove(Cube& c) {
	//r now means u
	int temp = c.moveConversions[6];
	c.moveConversions[6] = c.moveConversions[0];
	c.moveConversions[0] = c.moveConversions[15];
	c.moveConversions[15] = c.moveConversions[9];
	c.moveConversions[9] = temp;

	temp = c.moveConversions[7];
	c.moveConversions[7] = c.moveConversions[1];
	c.moveConversions[1] = c.moveConversions[16];
	c.moveConversions[16] = c.moveConversions[10];
	c.moveConversions[10] = temp;

	temp = c.moveConversions[8];
	c.moveConversions[8] = c.moveConversions[2];
	c.moveConversions[2] = c.moveConversions[17];
	c.moveConversions[17] = c.moveConversions[11];
	c.moveConversions[11] = temp;

	c.topFace = getNextTopFace(c.topFace, c.frontFace);
}

int Cube::getNextFrontFace(int topFace, int frontFace) {
	//cout << "in next front face\n";
	//cout << "called with tf " << topFace << " ff " << frontFace << '\n';
	int i = 0;
	while (faceChanges[topFace][i] != frontFace && i < 3) {
		i++;
	}
	return faceChanges[topFace][(i + 1) % 4];
}
int Cube::getNextTopFace(int topFace, int frontFace) {
	int i = 0;
	while (faceChanges[frontFace][i] != topFace && i < 3) {
		i++;
	}
	return faceChanges[frontFace][(i + 1) % 4];
}