#include "Cube.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
using std::cout;
using std::string;

const int Cube::faceChanges[6][4] = {
		{1, 2, 3, 4},
		{0, 4, 5, 2},
		{0, 1, 5, 3},
		{0, 2, 5, 4},
		{0, 3, 5, 1},
		{1, 4, 3, 2},
};
const int Cube::opposites[6] = { 5, 3, 4, 1, 2, 0 };
const int Cube::xmoveFacesChanges[6] = { 1, 5, 2, 0, 4, 3 };
const int Cube::ymoveFacesChanges[6] = { 0, 2, 3, 4, 1, 5 };
const int Cube::zmoveFacesChanges[6] = { 4, 1, 0, 3, 5, 2 };


Cube::Cube() {
	for (int i = 0; i < 8; i++) {
		corners[i] = Cuboid(i, 0);
	}
	frontFace = 1;
	topFace = 0;
}

Cube::Cube(Cube* c) {
	for (int i = 0; i < 8; i++) {
		corners[i] = Cuboid(c->corners[i]);
	}
	frontFace = 1;
	topFace = 0;
}

Cube::Cube(int index) {
	//NOT WORKING
	int total = 3674160;
	int divider = 7;
	bool visitied[8] = { false, false, false, false, false, false, false, false };
	for (int i = 0; i < 6; ++i) {
		total /= divider;
		int thisCorner = index / total;
		//find which corner that is actually

		index %= total;
		total /= 3;
		corners[i].orientation = index / total;
		index %= 3;
		--divider;
	}

	corners[6].identity = 6;
	corners[6].orientation = 0;

	//find 7
	bool done[8] = { false, false, false, false, false, false, false, false };
	for (int i = 0; i < 7; ++i) {
		done[corners[i].identity] = true;
	}

	int notYet = 0;
	while (done[notYet]) {
		++notYet;
	}

	corners[7].identity = notYet;
	int ors = 0;
	for (int i = 0; i < 6; ++i) {
		ors += corners[i].orientation;
	}

	corners[7].orientation = (3 - (ors % 3)) % 3;

	frontFace = 1;
	topFace = 0;
}

string rotateString(string s) {
	string ns = "";
	ns += s.at(1); ns += s.at(2); ns += s.at(0);
	return ns;
}

string makeWorYFirstChar(string s) {
	for (int i = 0; i < 3; i++) {
		if (s.at(0) == 'w' || s.at(0) == 'y') {
			return s;
		}
		s = rotateString(s);
	}
	cout << "FISR T " << s.at(0) << '\n';
	cout << "TGBHFUSIOPNJMOKHJ,PLRFVYTGIBUHONIJPMOK,LRTFUYVIGBUOHNIJPMOK[,PLhere wasn't a yellow or white face on this corner! AHHIJFEANJFJKSDBNAFLJEWHThats bad!!\nPiece had the colours " << s << '\n';
	return "";
}

int numOfRotates(string s) {
	for (int i = 0; i < 3; i++) {
		if (s.at(0) == 'w' || s.at(0) == 'y') {
			return i;
		}
		s = rotateString(s);
	}
	cout << "There wasn't a yellow or white face on this corner! Thats bad!! (num rotates)\nPiece had the colours " << s << '\n';
	return -1;
}

int getIdentityoffString(string s) {
	string identities[8] = { "wob", "wbr", "wgo", "wrg", "yog", "ygr", "ybo", "yrb" };
	for (int i = 0; i < 8; i++) {
		if (s == identities[i]) {
			return i;
		}
	}
	cout << "not an acutal piece! s was " << s << "\n";
	return -1;
}
string swapLastTwo(string s) {
	string ns = "";
	ns += s.at(0); ns += s.at(2); ns += s.at(1);
	return ns;
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

Cube Cube::createCubeFromFaces() {
	cout << "Please orientate the cube so the yellow-orange-blue corner has yellow facing down and is in the back bottom left corner of the cube\n";
	//cout << "Enter the top face in clockwise order (ex - ygob)\n";
	std::string fc[6] = {"top", "front", "right", "back", "left", "bottom"};
	
	
	char faces[6][4] = {
		{'o', 'y', 'w', 'y',},
		{'g', 'r', 'y', 'g',},
		{'g', 'b', 'w', 'b',},
		{'r', 'w', 'r', 'b',},
		{'b', 'o', 'o', 'g',},
		{'o', 'r', 'y', 'w',},
	};
	/*{
		{'w', 'w', 'w', 'w',},
		{'g', 'g', 'g', 'g',},
		{'r', 'r', 'r', 'r',},
		{'b', 'b', 'b', 'b',},
		{'o', 'o', 'o', 'o',},
		{'y', 'y', 'y', 'y',},
	};*/
	for (int i = 0; i < 6; i++) {
		
		cout << "Enter the " << fc[i] << " face in clockwise order(ex - ygob)\n";
		std::string s;
		std::cin >> s;
		faces[i][0] = s.at(0);
		faces[i][1] = s.at(1);
		faces[i][2] = s.at(3);
		faces[i][3] = s.at(2);
		
	}

	printFaces(faces);

	
	//convert from the faces array to the cube
	Cube c = Cube();
	string firstCorner = "";
	firstCorner += faces[0][0];
	firstCorner += faces[4][0];
	firstCorner += faces[3][1];
	cout << firstCorner << " was first c\n";
	string fixed = makeWorYFirstChar(firstCorner);
	cout << "fixed: " << fixed << '\n';
	
	c.corners[0] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));
	cout << "corner done 1\n";
	firstCorner = "";
	firstCorner += faces[0][1]; firstCorner += faces[3][0]; firstCorner += faces[2][1];
	fixed = makeWorYFirstChar(firstCorner);
	c.corners[1] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));
	firstCorner = ""; firstCorner += faces[0][2]; firstCorner += faces[1][0]; firstCorner += faces[4][1];
	fixed = makeWorYFirstChar(firstCorner);
	c.corners[2] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));
	firstCorner = ""; firstCorner += faces[0][3]; firstCorner += faces[2][0]; firstCorner += faces[1][1];
	fixed = makeWorYFirstChar(firstCorner);
	c.corners[3] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));

	firstCorner = ""; firstCorner += faces[5][0]; firstCorner += faces[4][3]; firstCorner += faces[1][2];
	fixed = makeWorYFirstChar(firstCorner);
	c.corners[4] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));
	firstCorner = ""; firstCorner += faces[5][1]; firstCorner += faces[1][3]; firstCorner += faces[2][2];
	fixed = makeWorYFirstChar(firstCorner);
	c.corners[5] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));
	firstCorner = ""; firstCorner += faces[5][2]; firstCorner += faces[3][3]; firstCorner += faces[4][2];
	fixed = makeWorYFirstChar(firstCorner);
	c.corners[6] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));
	firstCorner = ""; firstCorner += faces[5][3]; firstCorner += faces[2][3]; firstCorner += faces[3][2];
	fixed = makeWorYFirstChar(firstCorner);
	c.corners[7] = Cuboid(getIdentityoffString(fixed), numOfRotates(firstCorner));


	cout << "all done\n";
	return c;
}

bool Cube::isSolved(Cube c) {
	for (int i = 0; i < 8; i++) {
		if (c.corners[i].identity != i || c.corners[i].orientation != 0) {
			return false;
		}
	}
	return true;
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
	faces[1][0] = faces[1][2];
	faces[1][2] = faces[1][3];
	faces[1][3] = faces[1][1];
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

void Cube::print(Cube c) {
	std::cout << "\n\n\n";
	/*
	cout << "Move conversions: (input - output)\n";
	for (int i = 0; i < 18; i+=3) {
		cout << getMoveInStrForm(i) << " is now " << getMoveInStrForm(c.moveConversions[i]) << '\n';
	}
	cout << "\nTop Face - " << c.topFace << "\nFront Face - " << c.frontFace << "\n\n";
	*/
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
	/*
	for (int i = 0; i < 8; i++) {
		std::cout << int(c.corners[i].identity) << ' ' << int(c.corners[i].orientation) << std::endl;
	}
	*/

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
	//cout << "before rotations\n";
	//printFaces(faces);
	//cout << "starting cycle\n";
	while (cycle[tf] != c.topFace && cycle[ff] != c.frontFace) {
		tf++;
		tf %= 4;
		ff++;
		ff %= 4;
		xRotateFaces(faces);
		//cout << "x rotating once\n";
		//printFaces(faces);
	}
	//cout << "done cycle\n";

	if (cycle[tf] == c.topFace) {
		ff = cycle[ff];
		while (ff != c.frontFace) {
			ff = getNextFrontFace(cycle[tf], ff);
			yRotateFaces(faces);
			//cout << "y rotating once\n";
			//printFaces(faces);
		}
	}
	else {
		tf = cycle[tf];
		while (tf != c.topFace) {
			tf = getNextTopFace(tf, cycle[ff]);
			zRotateFaces(faces);
			//cout << "z rotating once\n";
			//printFaces(faces);
		}
	}
	
	//cout << "final print\n";
	//print the faces	
	//w 
	printFaces(faces);
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
	//cout << "move was " << choice << '\n';
	
	choice = c.moveConversions[choice];
	//cout << "move now is " << choice << '\n';
	c.move(choice);
	//doRotationOnMove(c, choice);
}

std::string Cube::getMoveInStrForm(int n) {
	switch (n) {
	case 0:
		return "U";
	case 1:
		return "U2";
	case 2:
		return "U'";
	case 3:
		return "F";
	case 4:
		return "F2";
	case 5:
		return "F'";
	case 6:
		return "R";
	case 7:
		return "R2";
	case 8:
		return "R'";
	case 9://d
		return "D";
	case 10://d2
		return "D2";
	case 11://dp
		return "D'";
	case 12://b
		return "B";
	case 13://b2
		return "B2";
	case 14://bp
		return "B'";
	case 15://l
		return "L";
	case 16://l2
		return "L2";
	case 17://lp
		return "L'";
	default:
		return  "Not a move!";
	}
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
void Cube::unmove(int move) {
	switch (move) {
	case 0:
		uprime();
		break;
	case 1:
		u2move();
		break;
	case 2:
		umove();
		break;
	case 3:
		fprime();
		break;
	case 4:
		f2move();
		break;
	case 5:
		fmove();
		break;
	case 6:
		rprime();
		break;
	case 7:
		r2move();
		break;
	case 8:
		rmove();
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
	/*
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
	*/

	//u now means f
	//f now means d
	//d now means b
	//b now means u

	int newConversions[18];
	for (int i = 0; i < 18; i += 3) {
		newConversions[i] = 0; // get rid of that warning >:(
		switch (c.moveConversions[i]) {
		case 0:
			newConversions[i] = 3;
			break;
		case 3:
			newConversions[i] = 9;
			break;
		case 6:
			newConversions[i] = 6;
			break;
		case 9:
			newConversions[i] = 12;
			break;
		case 12:
			newConversions[i] = 0;
			break;
		case 15:
			newConversions[i] = 15;
			break;
		default:
			cout << "AHHHHHHHH smt happened in xmove that wasnt supposed to happend!!!\n";
			cout << "Move conversions at pos " << i << " was somehow " << c.moveConversions[i] << '\n';
		}
		newConversions[i + 1] = newConversions[i] + 1;
		newConversions[i + 2] = newConversions[i] + 2;
	}

	for (int i = 0; i < 18; i++) {
		c.moveConversions[i] = newConversions[i];
	}

	c.topFace = Cube::xmoveFacesChanges[c.topFace];
	c.frontFace = Cube::xmoveFacesChanges[c.frontFace];
}
void Cube::ymove(Cube& c) {
	//cout << "called ymove\n";
	/*
	//f now means r
	int temp = c.moveConversions[3];
	c.moveConversions[3] = c.moveConversions[6];
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
	*/



	//f now means r
	//r now means b
	//b now means l
	//l now means f

	int newConversions[18];
	for (int i = 0; i < 18; i += 3) {
		newConversions[i] = 0; // get rid of that warning >:(
		switch (c.moveConversions[i]) {
		case 0:
			newConversions[i] = 0;
			break;
		case 3:
			newConversions[i] = 6;
			break;
		case 6:
			newConversions[i] = 12;
			break;
		case 9:
			newConversions[i] = 9;
			break;
		case 12:
			newConversions[i] = 15;
			break;
		case 15:
			newConversions[i] = 3;
			break;
		default:
			cout << "AHHHHHHHH smt happened in ymove that wasnt supposed to happend!!!\n";
			cout << "Move conversions at pos " << i << " was somehow " << c.moveConversions[i] << '\n';
		}
		newConversions[i + 1] = newConversions[i] + 1;
		newConversions[i + 2] = newConversions[i] + 2;
	}

	for (int i = 0; i < 18; i++) {
		c.moveConversions[i] = newConversions[i];
	}

	c.topFace = Cube::ymoveFacesChanges[c.topFace];
	c.frontFace = Cube::ymoveFacesChanges[c.frontFace];
}
void Cube::zmove(Cube& c) {
	//cout << "called zmove\n";
	/*
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
	*/
	
	//u now means l
	//l now means d
	//d now means r
	//r now means u

	int newConversions[18];
	for (int i = 0; i < 18; i += 3) {
		newConversions[i] = 0; // get rid of that warning >:(
		switch (c.moveConversions[i]) {
		case 0:
			newConversions[i] = 15;
			break;
		case 3:
			newConversions[i] = 3;
			break;
		case 6:
			newConversions[i] = 0;
			break;
		case 9:
			newConversions[i] = 6;
			break;
		case 12:
			newConversions[i] = 12;
			break;
		case 15:
			newConversions[i] = 9;
			break;
		default:
			cout << "AHHHHHHHH smt happened in zmove that wasnt supposed to happend!!!\n";
			cout << "Move conversions at pos " << i << " was somehow " << c.moveConversions[i] << '\n';
		}
		newConversions[i + 1] = newConversions[i] + 1;
		newConversions[i + 2] = newConversions[i] + 2;
	}

	cout << "news\n";
	for (int i = 0; i < 18; i++) {
		cout << newConversions[i] << ' ';
	}
	cout << '\n';

	for (int i = 0; i < 18; i++) {
		c.moveConversions[i] = newConversions[i];
	}

	cout << "olds\n";
	for (int i = 0; i < 18; i++) {
		cout << c.moveConversions[i] << ' ';
	}
	cout << '\n';

	c.topFace = Cube::zmoveFacesChanges[c.topFace];
	c.frontFace = Cube::zmoveFacesChanges[c.frontFace];
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