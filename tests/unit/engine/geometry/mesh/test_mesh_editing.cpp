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
    MeshData extrusion_mesh;
    const auto e0 = extrusion_mesh.add_vertex({0.0F, 0.0F, 0.0F});
    const auto e1 = extrusion_mesh.add_vertex({2.0F, 0.0F, 0.0F});
    const auto e2 = extrusion_mesh.add_vertex({2.0F, 2.0F, 0.0F});
    const auto e3 = extrusion_mesh.add_vertex({0.0F, 2.0F, 0.0F});
    assert(e0 && e1 && e2 && e3);
    const auto base_face = extrusion_mesh.add_face({*e0, *e1, *e2, *e3});
    assert(base_face);
    assert(extrusion_mesh.extrude_face(*base_face, {0.0F, 0.0F, 1.5F}));
    assert(extrusion_mesh.vertices().size() == 8U);
    assert(extrusion_mesh.edges().size() == 8U);
    assert(extrusion_mesh.faces().size() == 5U);
    assert(extrusion_mesh.face(*base_face)->vertices.size() == 4U);
    assert(extrusion_mesh.vertex(extrusion_mesh.face(*base_face)->vertices[0])
               ->position.nearly_equals({0.0F, 0.0F, 1.5F}));
    assert(extrusion_mesh.validate());

    MeshData inset_mesh;
    const auto i0 = inset_mesh.add_vertex({0.0F, 0.0F, 0.0F});
    const auto i1 = inset_mesh.add_vertex({2.0F, 0.0F, 0.0F});
    const auto i2 = inset_mesh.add_vertex({2.0F, 2.0F, 0.0F});
    const auto i3 = inset_mesh.add_vertex({0.0F, 2.0F, 0.0F});
    assert(i0 && i1 && i2 && i3);
    const auto outer_face = inset_mesh.add_face({*i0, *i1, *i2, *i3});
    assert(outer_face);
    assert(!inset_mesh.inset_face(*outer_face, 0.0F));
    assert(!inset_mesh.inset_face(*outer_face, 1.0F));
    assert(inset_mesh.inset_face(*outer_face, 0.25F));
    assert(inset_mesh.vertices().size() == 8U);
    assert(inset_mesh.edges().size() == 8U);
    assert(inset_mesh.faces().size() == 5U);
    const auto* inner_face = inset_mesh.face(*outer_face);
    assert(inner_face != nullptr);
    assert(inset_mesh.vertex(inner_face->vertices[0])
               ->position.nearly_equals({0.5F, 0.5F, 0.0F}));
    assert(inset_mesh.validate());

}
