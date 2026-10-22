#include <cassert>

#include "physics.h"

void RigidBody::calc_derivatives() {
	orientation.normalize();
	calc_transform_mat(transform_mat, position_world, orientation);
	transform_inertia_tensor(
		inverse_inertia_tensor_world,
		orientation,
		inverse_inertia_tensor,
		transform_mat
	);
}

void RigidBody::set_mass(float mass) {
	assert(mass != 0.0f);
	inverse_mass = 1.0f / mass;
}

void RigidBody::set_inertia_tensor(const Mat3& inertia_tensor) {
	inverse_inertia_tensor = Mat3::inverse(inertia_tensor);
}

void RigidBody::add_force_at_point(const Vec3f& force_world, const Vec3f& point_world) {
	Vec3f point_local = point_world;
	point_local = point_local - position_world;

	net_force = net_force + force_world;
	net_torque = net_torque + Vec3f::cross(force_world, point_local);
}

void RigidBody::set_position(const Vec3f& pos) {
	this->position_world = pos;
}

Mat4f RigidBody::get_tranform() const {
	return transform_mat;
}

void RigidBody::clear_accumulators() {
	net_force = Vec3f::zero;
	net_torque = Vec3f::zero;
}

void RigidBody::integrate(float dt) {
	Vec3f linear_accel = (net_force * inverse_mass) /*+ Vec3f(0.0f, -9.8f, 0.0f)*/;
	position_world = position_world + (linear_velocity_world * dt);

	Vec3f angular_accel_world = inverse_inertia_tensor_world * net_torque;
	orientation.add_scaled_vector(angular_velocity_world, dt);

	linear_velocity_world = linear_velocity_world + (linear_accel * dt);
	angular_velocity_world = angular_velocity_world + (angular_accel_world * dt);

	calc_derivatives();
	clear_accumulators();
}

void RigidBody::calc_transform_mat(Mat4f& transform_matrix, const Vec3f& position_world, const Quat& orientation) {
	transform_matrix = Mat4f::create_translation(position_world) * Mat4f::create_rotation(orientation);
}

// I_world^-1 = R * I_body^-1 * R^T
void RigidBody::transform_inertia_tensor(Mat3& iit_world, const Quat& q, const Mat3& iit_body, const Mat4f& rotmat) {
	Mat3 rot = rotmat.get_upper_left_mat3();
	iit_world = rot * iit_body * Mat3::transpose(rot);
}
