#include "../../../../../engine/geometry/mesh/mesh_data.hpp"

#include <cassert>
#include <string>

using anvil::geometry::MeshData;
using anvil::math::Vector3;

int main() {
    MeshData mesh;

    const auto a = mesh.add_vertex({0.0F, 0.0F, 0.0F});
    const auto b = mesh.add_vertex({1.0F, 0.0F, 0.0F});
    const auto c = mesh.add_vertex({1.0F, 1.0F, 0.0F});
    const auto d = mesh.add_vertex({0.0F, 1.0F, 0.0F});

    assert(a && b && c && d);
    assert(*a != *b && *b != *c && *c != *d);
    const auto ab = mesh.add_edge(*a, *b);
    assert(ab);
    assert(mesh.add_edge(*b, *c));
    assert(mesh.add_edge(*c, *d));
    assert(mesh.add_edge(*d, *a));
    const auto quad = mesh.add_face({*a, *b, *c, *d});
    assert(quad);

    assert(!mesh.add_edge(*a, *a));
    assert(!mesh.add_edge(*a, 9999U));
    assert(!mesh.add_face({*a, *b}));
    assert(!mesh.add_face({*a, *b, *b}));

    assert(mesh.vertices().size() == 4U);
    assert(mesh.edges().size() == 4U);
    assert(mesh.faces().size() == 1U);
    assert(mesh.vertex(*c) != nullptr);
    assert(mesh.vertex(9999U) == nullptr);
    assert(mesh.edge(*ab) != nullptr);
    assert(mesh.face(*quad) != nullptr);

    assert(mesh.set_vertex_position(*c, {2.0F, 2.0F, 0.0F}));
    assert(mesh.vertex(*c)->position.nearly_equals({2.0F, 2.0F, 0.0F}));
    assert(!mesh.set_vertex_position(9999U, {}));

    std::string reason = "not cleared";
    assert(mesh.validate(&reason));
    assert(reason.empty());

    mesh.clear();
    assert(mesh.vertices().empty());
    assert(mesh.edges().empty());
    assert(mesh.faces().empty());
    assert(mesh.validate());

    // Old IDs must not be reused after clear; stale handles stay invalid.
    const auto fresh = mesh.add_vertex({3.0F, 0.0F, 0.0F});
    assert(fresh && *fresh != *a);
    assert(mesh.vertex(*a) == nullptr);
    assert(mesh.vertex(*fresh) != nullptr);
    assert(mesh.validate());
}
