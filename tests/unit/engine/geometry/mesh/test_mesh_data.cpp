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
    assert(mesh.add_edge(*a, *b));
    assert(mesh.add_edge(*b, *c));
    assert(mesh.add_edge(*c, *d));
    assert(mesh.add_edge(*d, *a));
    assert(mesh.add_face({*a, *b, *c, *d}));
    assert(!mesh.add_edge(*a, *a));
    assert(!mesh.add_edge(*a, 99U));
    assert(!mesh.add_face({*a, *b}));
    assert(!mesh.add_face({*a, *b, *b}));

    assert(mesh.vertices().size() == 4U);
    assert(mesh.edges().size() == 4U);
    assert(mesh.faces().size() == 1U);
    assert(mesh.vertex(*c) != nullptr);
    assert(mesh.vertex(99U) == nullptr);
    assert(mesh.face(0U) != nullptr);

    std::string reason = "not cleared";
    assert(mesh.validate(&reason));
    assert(reason.empty());

    mesh.clear();
    assert(mesh.vertices().empty());
    assert(mesh.edges().empty());
    assert(mesh.faces().empty());
    assert(mesh.validate());
}
