#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    
    float dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
    
    Vector3 normalize() const {
        float len = std::sqrt(x * x + y * y + z * z);
        return Vector3(x / len, y / len, z / len);
    }
};

#endif
