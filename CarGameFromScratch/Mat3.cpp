#include <algorithm>

#include "Mat3.h"
#include "Vec3f.h"

// Identity.
Mat3::Mat3() {
	std::ranges::fill(data, 0.0f);

	data[0] = 1.0f;
	data[4] = 1.0f;
	data[8] = 1.0f;
}

Mat3::Mat3(const Vec3f& col_0, const Vec3f& col_1, const Vec3f& col_2) {
	data = {
		col_0.x, col_0.y, col_0.z,
		col_1.x, col_1.y, col_1.z,
		col_2.x, col_2.y, col_2.z
	};
}

Mat3 Mat3::operator*(const Mat3& mat_right) const {
	Mat3 prod;
	for (int col = 0; col < 3; ++col) {
		for (int row = 0; row < 3; ++row) {
			float sum = 0.0f;
			for (int i = 0; i < 3; ++i) {
				sum += data[i * 3 + row] * mat_right.data[col * 3 + i];
			}
			prod.data[col * 3 + row] = sum;
		}
	}
	return prod;
}

Vec3f Mat3::operator*(const Vec3f& vec) const {
	return Vec3f(
		data[0] * vec.x + data[3] * vec.y + data[6] * vec.z,
		data[1] * vec.x + data[4] * vec.y + data[7] * vec.z,
		data[2] * vec.x + data[5] * vec.y + data[8] * vec.z
	);
}

Mat3 Mat3::create_identity() {
    return Mat3();
}

Mat3 Mat3::inverse(const Mat3& mat) {
	const std::array<float, 9>& m = mat.data;

	float a = m[0], b = m[3], c = m[6];
	float d = m[1], e = m[4], f = m[7];
	float g = m[2], h = m[5], i = m[8];

	float A =  (e * i - f * h);
	float B = -(d * i - f * g);
	float C =  (d * h - e * g);

	float det = a * A + b * B + c * C;
	if (det == 0.0f) {
		return Mat3();
	}

	float D = -(b * i - c * h);
	float E =  (a * i - c * g);
	float F = -(a * h - b * g);
	float G =  (b * f - c * e);
	float H = -(a * f - c * d);
	float I =  (a * e - b * d);

	float inv_det = 1.0f / det;
	Mat3 result;
	result.data = {
		A * inv_det, B * inv_det, C * inv_det,
		D * inv_det, E * inv_det, F * inv_det,
		G * inv_det, H * inv_det, I * inv_det
	};
	return result;
}

Mat3 Mat3::transpose(const Mat3& mat) {
	Mat3 result;
	for (int col = 0; col < 3; ++col) {
		for (int row = 0; row < 3; ++row) {
			result.data[row * 3 + col] = mat.data[col * 3 + row];
		}
	}
	return result;
}
