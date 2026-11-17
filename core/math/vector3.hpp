#pragma once

#include <cmath>
#include <stdexcept>

namespace anvil::math {

struct Vector3 {
    float x{0.0F};
    float y{0.0F};
    float z{0.0F};

    constexpr Vector3() = default;
    constexpr Vector3(float x_value, float y_value, float z_value)
        : x(x_value), y(y_value), z(z_value) {}

    [[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    [[nodiscard]] constexpr Vector3 operator-(const Vector3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    [[nodiscard]] constexpr Vector3 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    [[nodiscard]] constexpr Vector3 operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    constexpr Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    [[nodiscard]] constexpr float length_squared() const {
        return x * x + y * y + z * z;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(length_squared());
    }

    [[nodiscard]] constexpr float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    [[nodiscard]] constexpr Vector3 cross(const Vector3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    [[nodiscard]] Vector3 normalized(float epsilon = 1.0e-6F) const {
        const float magnitude = length();
        if (magnitude <= epsilon) {
            return {};
        }
        return *this / magnitude;
    }

    [[nodiscard]] constexpr bool nearly_equals(
        const Vector3& other,
        float tolerance = 1.0e-5F
    ) const {
        const auto close = [tolerance](float a, float b) {
            const float difference = a - b;
            return difference >= -tolerance && difference <= tolerance;
        };
        return close(x, other.x) && close(y, other.y) && close(z, other.z);
    }
};

[[nodiscard]] constexpr Vector3 operator*(float scalar, const Vector3& vector) {
    return vector * scalar;
}

} // namespace anvil::math
