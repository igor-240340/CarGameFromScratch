#pragma once

#include "Vec3f.h"
#include "Quat.h"

class RigidBody {
public:

private:
	float inverse_mass = 0.0f;
	Vec3f position;
	Quat orientation;
	Vec3f linear_velocity;
};
