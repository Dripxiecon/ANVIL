#pragma once

#include "edge.hpp"

#include <vector>

namespace anvil::geometry {

struct Face {
    FaceId id{invalid_face_id};
    // Stable vertex IDs ordered around the face boundary.
    std::vector<VertexId> vertices;
};

} // namespace anvil::geometry
