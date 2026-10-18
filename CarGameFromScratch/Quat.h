#pragma once

class Vec3f;

// Rotation quaternion: w + xi + yj + zk. Valid rotation only when normalized.
class Quat {
public:
	Quat() = default;
	constexpr Quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

	void normalize();

	Quat operator*(const Quat& b) const;

	void add_scaled_vector(const Vec3f& vec, float scale);
	void rotate_by_vector(const Vec3f& vec);

public:
	float w = 1.0f;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};
