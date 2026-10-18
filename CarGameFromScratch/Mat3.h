#pragma once

#include <array>

class Vec3f;

// Column-major order.
class Mat3 {
public:
	Mat3();
	Mat3(const Vec3f& col_0, const Vec3f& col_1, const Vec3f& col_2);

	Mat3 operator*(const Mat3& mat) const;

	static Mat3 create_identity();
	static Mat3 inverse(const Mat3& mat);
	static Mat3 transpose(const Mat3& mat);

private:
	std::array<float, 9> data;
};
