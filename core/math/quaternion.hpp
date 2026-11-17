#pragma once

#include "vector3.hpp"

#include <cmath>

namespace anvil::math {

struct Quaternion {
    float x{0.0F};
    float y{0.0F};
    float z{0.0F};
    float w{1.0F};

    constexpr Quaternion() = default;
    constexpr Quaternion(float x_value, float y_value, float z_value, float w_value)
        : x(x_value), y(y_value), z(z_value), w(w_value) {}

    [[nodiscard]] static Quaternion from_axis_angle(
        const Vector3& axis,
        float radians
    ) {
        const Vector3 direction = axis.normalized();
        if (direction.length_squared() == 0.0F) {
            return {};
        }

        const float half_angle = radians * 0.5F;
        const float sine = std::sin(half_angle);
        return {
            direction.x * sine,
            direction.y * sine,
            direction.z * sine,
            std::cos(half_angle)
        };
    }

    [[nodiscard]] constexpr Quaternion conjugate() const {
        return {-x, -y, -z, w};
    }

    [[nodiscard]] constexpr float length_squared() const {
        return x * x + y * y + z * z + w * w;
    }

    [[nodiscard]] Quaternion normalized(float epsilon = 1.0e-6F) const {
        const float magnitude = std::sqrt(length_squared());
        if (magnitude <= epsilon) {
            return {};
        }
        return {x / magnitude, y / magnitude, z / magnitude, w / magnitude};
    }

    [[nodiscard]] constexpr Quaternion operator*(const Quaternion& other) const {
        return {
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        };
    }

    [[nodiscard]] Vector3 rotate(const Vector3& vector) const {
        const Quaternion unit = normalized();
        const Quaternion point{vector.x, vector.y, vector.z, 0.0F};
        const Quaternion result = unit * point * unit.conjugate();
        return {result.x, result.y, result.z};
    }
};

} // namespace anvil::math
