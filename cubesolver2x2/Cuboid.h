#pragma once
class Cuboid
{
public:
	Cuboid();
	Cuboid(int i, int o);
	unsigned int orientation : 3;
	unsigned int identity : 3;
	void add(int change);
};

