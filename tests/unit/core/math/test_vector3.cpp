#include "../../../../core/math/vector3.hpp"

#include <cassert>
#include <cmath>

using anvil::math::Vector3;

int main() {
    const Vector3 a{1.0F, 2.0F, 3.0F};
    const Vector3 b{3.0F, 2.0F, 1.0F};

    assert((a + b).nearly_equals({4.0F, 4.0F, 4.0F}));
    assert((a - b).nearly_equals({-2.0F, 0.0F, 2.0F}));
    assert(std::abs(a.dot(b) - 10.0F) < 1.0e-5F);
    assert(a.cross(b).nearly_equals({-4.0F, 8.0F, -4.0F}));
    assert(std::abs(a.normalized().length() - 1.0F) < 1.0e-5F);
    assert(Vector3{}.normalized().nearly_equals({}));
}
