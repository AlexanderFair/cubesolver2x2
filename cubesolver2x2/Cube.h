#pragma once
#include "Cuboid.h"
#include <iostream>


class Cube
{
public:
	Cube();
	
	static void print(Cube c);
	void move(int move);
	static void moveFromString(Cube& c, std::string s);

	
	
private:
	int moveConversions[18] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	static const int faceChanges[6][4];
	Cuboid corners[8];
	unsigned int frontFace : 3;
	unsigned int topFace : 3;

	void rmove();
	void r2move();
	void rprime();

	void umove();
	void u2move();
	void uprime();

	void fmove();
	void f2move();
	void fprime();

	static void xmove(Cube& c);
	static void ymove(Cube& c);
	static void zmove(Cube& c);

	static int getNextFrontFace(int topFace, int frontFace);
	static int getNextTopFace(int topFace, int frontFace);
};

