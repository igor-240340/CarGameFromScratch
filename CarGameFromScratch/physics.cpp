#include "physics.h"

void RigidBody::calc_derived_data() {
	orientation.normalize();
	calc_transform_mat(transform_mat, position_world, orientation);
	transform_inertia_tensor(
		inverse_inertia_tensor_world,
		orientation,
		inverse_inertia_tensor,
		transform_mat
	);
}

void RigidBody::set_inertia_tensor(const Mat3& inertia_tensor) {
	inverse_inertia_tensor = Mat3::inverse(inertia_tensor);
}

void RigidBody::add_force_at_point(const Vec3f& force_world, const Vec3f& point_world) {
	Vec3f point_local = point_world;
	point_local = point_local - position_world;

	force_accum = force_accum + force_world;
	torque_accum = torque_accum + Vec3f::cross(force_world, point_local);
}

void RigidBody::clear_accumulators() {
	force_accum = Vec3f::zero;
	torque_accum = Vec3f::zero;
}

void RigidBody::integrate(float dt) {
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
