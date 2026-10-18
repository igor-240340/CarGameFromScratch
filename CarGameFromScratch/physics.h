#pragma once

#include "Vec3f.h"
#include "Mat4f.h"
#include "Mat3.h"
#include "Quat.h"

class RigidBody {
public:
	void calc_derived_data();
	void set_inertia_tensor(const Mat3& inertia_tensor);

private:
	void calc_transform_mat(Mat4f& transform_matrix, const Vec3f& position, const Quat& orientation);
	void transform_inertia_tensor(
		Mat3& iit_world,
		const Quat& q,
		const Mat3& iit_body,
		const Mat4f& rotmat
	);

private:
	float inverse_mass = 0.0f;
	Mat3 inverse_inertia_tensor;
	Mat3 inverse_inertia_tensor_world;

	Vec3f position;
	Quat orientation;
	Vec3f linear_velocity;
	Mat4f transform_mat;
};
