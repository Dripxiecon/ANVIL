#include "../../../../core/math/matrix4.hpp"

#include <cassert>

using anvil::math::Matrix4;
using anvil::math::Quaternion;
using anvil::math::Vector3;

int main() {
    const Matrix4 model =
        Matrix4::translation({4.0F, 2.0F, 0.0F})
        * Matrix4::rotation(Quaternion::from_axis_angle(
            {0.0F, 0.0F, 1.0F},
            1.57079632679F
        ))
        * Matrix4::scaling({2.0F, 2.0F, 2.0F});

    assert(model.transform_point({1.0F, 0.0F, 0.0F})
        .nearly_equals({4.0F, 4.0F, 0.0F}, 1.0e-4F));
    assert(model.transform_direction({1.0F, 0.0F, 0.0F})
        .nearly_equals({0.0F, 2.0F, 0.0F}, 1.0e-4F));
    assert(Matrix4::identity().transform_point({1.0F, 2.0F, 3.0F})
        .nearly_equals({1.0F, 2.0F, 3.0F}));
}
