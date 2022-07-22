#include "Cuboid.h"

Cuboid::Cuboid() {
	identity = 0;
	orientation = 0;
}

Cuboid::Cuboid(Cuboid* c) {
	identity = c->identity;
	orientation = c->orientation;
}

Cuboid::Cuboid(int i, int o) : identity(i), orientation(o) {}

void Cuboid::add(int change) {
	orientation += change;
	if (orientation > 2) {
		orientation -= 3;
	}
}