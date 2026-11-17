#include "../../../../core/math/transform.hpp"

#include <cassert>

using anvil::math::Quaternion;
using anvil::math::Transform;
using anvil::math::Vector3;

int main() {
    Transform transform;
    transform.position = {4.0F, 2.0F, 0.0F};
    transform.rotation = Quaternion::from_axis_angle(
        {0.0F, 0.0F, 1.0F},
        1.57079632679F
    );
    transform.scale = {2.0F, 2.0F, 2.0F};

    const Vector3 local{1.0F, 0.0F, 0.0F};
    const Vector3 world = transform.transform_point(local);
    assert(world.nearly_equals({4.0F, 4.0F, 0.0F}, 1.0e-4F));
    assert(transform.inverse_transform_point(world).nearly_equals(local, 1.0e-4F));
}
