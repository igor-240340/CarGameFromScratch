#pragma once

#include "Vec3f.h"
#include "Mat4f.h"
#include "Mat3.h"
#include "Quat.h"

class RigidBody {
public:
	void calc_derivatives();
	void set_mass(float mass);
	void set_inertia_tensor(const Mat3& inertia_tensor);
	void add_force_at_point(const Vec3f& force_world, const Vec3f& point_world);
	
	void set_position(const Vec3f& pos);
	Mat4f get_tranform() const;

	void clear_accumulators();
	void integrate(float dt);

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

	Vec3f position_world{};
	Quat orientation{};
	Vec3f linear_velocity_world{};
	Vec3f angular_velocity_world{};
	Mat4f transform_mat;

	Vec3f net_force{};
	Vec3f net_torque{};
};
