#pragma once

class Vec3f;

class Vec4f {
public:
    explicit Vec4f(const Vec3f& vec);
    Vec4f(float x, float y, float z, float w);

    Vec4f operator/(float scalar) const;

    static float dot(const Vec4f& a, const Vec4f& b);

public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
};
