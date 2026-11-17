#pragma once

#include "quaternion.hpp"

namespace anvil::math {

struct Transform {
    Vector3 position{};
    Quaternion rotation{};
    Vector3 scale{1.0F, 1.0F, 1.0F};

    [[nodiscard]] Vector3 transform_point(const Vector3& point) const {
        const Vector3 scaled{
            point.x * scale.x,
            point.y * scale.y,
            point.z * scale.z
        };
        return position + rotation.rotate(scaled);
    }

    [[nodiscard]] Vector3 transform_direction(const Vector3& direction) const {
        return rotation.rotate(direction);
    }

    [[nodiscard]] Vector3 inverse_transform_point(const Vector3& point) const {
        const Vector3 unrotated = rotation.normalized().conjugate().rotate(point - position);
        constexpr float epsilon = 1.0e-6F;
        return {
            std::abs(scale.x) <= epsilon ? 0.0F : unrotated.x / scale.x,
            std::abs(scale.y) <= epsilon ? 0.0F : unrotated.y / scale.y,
            std::abs(scale.z) <= epsilon ? 0.0F : unrotated.z / scale.z
        };
    }
};

} // namespace anvil::math
