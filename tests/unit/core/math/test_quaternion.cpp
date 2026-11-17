#include "../../../../core/math/quaternion.hpp"

#include <cassert>
#include <cmath>

using anvil::math::Quaternion;
using anvil::math::Vector3;

int main() {
    constexpr float half_pi = 1.57079632679F;
    const Quaternion rotation = Quaternion::from_axis_angle({0.0F, 0.0F, 1.0F}, half_pi);
    const Vector3 rotated = rotation.rotate({1.0F, 0.0F, 0.0F});

    assert(rotated.nearly_equals({0.0F, 1.0F, 0.0F}, 1.0e-4F));
    assert(Quaternion::from_axis_angle({}, half_pi).length_squared() == 1.0F);
}
