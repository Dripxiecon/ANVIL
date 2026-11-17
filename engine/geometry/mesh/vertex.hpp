#pragma once

#include "edge.hpp"
#include "../../../core/math/vector3.hpp"

namespace anvil::geometry {

struct Vertex {
    VertexId id{invalid_vertex_id};
    math::Vector3 position{};
};

} // namespace anvil::geometry
