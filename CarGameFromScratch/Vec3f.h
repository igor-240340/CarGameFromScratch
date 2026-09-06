#pragma once

class Vec4f;

class Vec3f {
public:
    static const Vec3f zero;
    static const Vec3f up;

public:
    Vec3f() = default;
    explicit Vec3f(const Vec4f& vec);
    constexpr Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

    float length() const;
    float length_squared() const;
    Vec3f get_normalized() const;

    Vec3f operator/(float scalar) const;
    Vec3f operator+(const Vec3f& b) const;
    Vec3f operator-(const Vec3f& b) const;
    Vec3f operator*(float scalar) const;
    Vec3f operator-() const;

    friend Vec3f operator*(float scalar, const Vec3f& vec);

    static float dot(const Vec3f& a, const Vec3f& b);
    static Vec3f cross(const Vec3f& a, const Vec3f& b);

public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

inline constexpr Vec3f Vec3f::zero(0.0f, 0.0f, 0.0f);
inline constexpr Vec3f Vec3f::up(0.0f, 1.0f, 0.0f);
