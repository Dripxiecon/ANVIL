#include "../../../../../engine/geometry/mesh/mesh_data.hpp"

#include <cassert>
#include <vector>

using anvil::geometry::MeshData;
using anvil::math::Vector3;

int main() {
    MeshData mesh;

    const auto a = mesh.add_vertex({0.0F, 0.0F, 0.0F});
    const auto b = mesh.add_vertex({2.0F, 0.0F, 0.0F});
    const auto c = mesh.add_vertex({1.0F, 1.0F, 0.0F});
    const auto d = mesh.add_vertex({1.0F, -1.0F, 0.0F});
    assert(a && b && c && d);

    const auto shared_edge = mesh.add_edge(*a, *b);
    const auto first_face = mesh.add_face({*a, *b, *c});
    const auto second_face = mesh.add_face({*b, *a, *d});
    assert(shared_edge && first_face && second_face);

    const auto middle = mesh.split_edge(*shared_edge, {1.0F, 0.0F, 0.0F});
    assert(middle);
    assert(mesh.vertex(*middle)->position.nearly_equals({1.0F, 0.0F, 0.0F}));

    // The original edge handle remains valid and identifies its first half.
    const auto* retained_edge = mesh.edge(*shared_edge);
    assert(retained_edge != nullptr);
    assert(retained_edge->first == *a);
    assert(retained_edge->second == *middle);

    const auto* left_face = mesh.face(*first_face);
    const auto* right_face = mesh.face(*second_face);
    assert(left_face != nullptr && right_face != nullptr);
    assert((left_face->vertices == std::vector<anvil::geometry::VertexId>{*a, *middle, *b, *c}));
    assert((right_face->vertices == std::vector<anvil::geometry::VertexId>{*b, *middle, *a, *d}));

    assert(mesh.edges().size() == 2U);
    assert(mesh.validate());
    assert(!mesh.split_edge(9999U, {}));
}
