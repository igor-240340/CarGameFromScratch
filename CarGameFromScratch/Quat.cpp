#include <cmath>
#include <limits>

#include "Quat.h"
#include "Vec3f.h"

void Quat::normalize() {
	float length_squared = w * w + x * x + y * y + z * z;

	if (length_squared < std::numeric_limits<float>::epsilon()) {
		*this = Quat();
		return;
	}

	float length_inv = 1.0f / std::sqrt(length_squared);
	w *= length_inv;
	x *= length_inv;
	y *= length_inv;
	z *= length_inv;
}

// (w1, v1) * (w2, v2) = (w1 * w2 - v1 . v2, w1 * v2 + w2 * v1 + v1 x v2)
Quat Quat::operator*(const Quat& b) const {
	Vec3f v_a(x, y, z);
	Vec3f v_b(b.x, b.y, b.z);

	float w_prod = w * b.w - Vec3f::dot(v_a, v_b);
	Vec3f v_prod = w * v_b + b.w * v_a + Vec3f::cross(v_a, v_b);

	return Quat(w_prod, v_prod.x, v_prod.y, v_prod.z);
}

// Integrates an angular velocity over time: q' = q + 0.5 * (0, vec * scale) * q.
void Quat::add_scaled_vector(const Vec3f& vec, float scale) {
	Vec3f v = vec * scale;
	Quat q = Quat(0.0f, v.x, v.y, v.z) * *this;

	w += 0.5f * q.w;
	x += 0.5f * q.x;
	y += 0.5f * q.y;
	z += 0.5f * q.z;
}

void Quat::rotate_by_vector(const Vec3f& vec) {
	*this = *this * Quat(0.0f, vec.x, vec.y, vec.z);
}
