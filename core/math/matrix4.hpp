#pragma once

#include "quaternion.hpp"

#include <array>
#include <cmath>

namespace anvil::math {

// Column-major 4x4 matrix, matching the layout expected by OpenGL.
struct Matrix4 {
    std::array<float, 16> values{};

    constexpr Matrix4() = default;

    [[nodiscard]] static constexpr Matrix4 identity() {
        Matrix4 result;
        result.values = {
            1.0F, 0.0F, 0.0F, 0.0F,
            0.0F, 1.0F, 0.0F, 0.0F,
            0.0F, 0.0F, 1.0F, 0.0F,
            0.0F, 0.0F, 0.0F, 1.0F
        };
        return result;
    }

    [[nodiscard]] static constexpr Matrix4 translation(const Vector3& offset) {
        Matrix4 result = identity();
        result.values[12] = offset.x;
        result.values[13] = offset.y;
        result.values[14] = offset.z;
        return result;
    }

    [[nodiscard]] static constexpr Matrix4 scaling(const Vector3& scale) {
        Matrix4 result = identity();
        result.values[0] = scale.x;
        result.values[5] = scale.y;
        result.values[10] = scale.z;
        return result;
    }

    [[nodiscard]] static Matrix4 rotation(const Quaternion& input) {
        const Quaternion q = input.normalized();
        const float xx = q.x * q.x;
        const float yy = q.y * q.y;
        const float zz = q.z * q.z;
        const float xy = q.x * q.y;
        const float xz = q.x * q.z;
        const float yz = q.y * q.z;
        const float wx = q.w * q.x;
        const float wy = q.w * q.y;
        const float wz = q.w * q.z;

        Matrix4 result = identity();
        result.values[0] = 1.0F - 2.0F * (yy + zz);
        result.values[1] = 2.0F * (xy + wz);
        result.values[2] = 2.0F * (xz - wy);
        result.values[4] = 2.0F * (xy - wz);
        result.values[5] = 1.0F - 2.0F * (xx + zz);
        result.values[6] = 2.0F * (yz + wx);
        result.values[8] = 2.0F * (xz + wy);
        result.values[9] = 2.0F * (yz - wx);
        result.values[10] = 1.0F - 2.0F * (xx + yy);
        return result;
    }

    [[nodiscard]] constexpr float at(std::size_t row, std::size_t column) const {
        return values[column * 4U + row];
    }

    [[nodiscard]] constexpr Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (std::size_t column = 0; column < 4U; ++column) {
            for (std::size_t row = 0; row < 4U; ++row) {
                float sum = 0.0F;
                for (std::size_t index = 0; index < 4U; ++index) {
                    sum += at(row, index) * other.at(index, column);
                }
                result.values[column * 4U + row] = sum;
            }
        }
        return result;
    }

    [[nodiscard]] Vector3 transform_point(const Vector3& point) const {
        const float x = at(0U, 0U) * point.x + at(0U, 1U) * point.y
            + at(0U, 2U) * point.z + at(0U, 3U);
        const float y = at(1U, 0U) * point.x + at(1U, 1U) * point.y
            + at(1U, 2U) * point.z + at(1U, 3U);
        const float z = at(2U, 0U) * point.x + at(2U, 1U) * point.y
            + at(2U, 2U) * point.z + at(2U, 3U);
        const float w = at(3U, 0U) * point.x + at(3U, 1U) * point.y
            + at(3U, 2U) * point.z + at(3U, 3U);

        if (std::abs(w) > 1.0e-6F && std::abs(w - 1.0F) > 1.0e-6F) {
            return {x / w, y / w, z / w};
        }
        return {x, y, z};
    }

    [[nodiscard]] Vector3 transform_direction(const Vector3& direction) const {
        return {
            at(0U, 0U) * direction.x + at(0U, 1U) * direction.y
                + at(0U, 2U) * direction.z,
            at(1U, 0U) * direction.x + at(1U, 1U) * direction.y
                + at(1U, 2U) * direction.z,
            at(2U, 0U) * direction.x + at(2U, 1U) * direction.y
                + at(2U, 2U) * direction.z
        };
    }
};

} // namespace anvil::math
