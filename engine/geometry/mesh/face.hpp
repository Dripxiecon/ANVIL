#pragma once

#include "edge.hpp"

#include <vector>

namespace anvil::geometry {

struct Face {
    // Vertex indices are ordered around the face boundary.
    std::vector<VertexIndex> vertices;
};

} // namespace anvil::geometry
