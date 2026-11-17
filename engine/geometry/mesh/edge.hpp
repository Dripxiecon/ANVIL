#pragma once

#include <cstdint>

namespace anvil::geometry {

using VertexId = std::uint64_t;
using EdgeId = std::uint64_t;
using FaceId = std::uint64_t;

inline constexpr VertexId invalid_vertex_id = 0;
inline constexpr EdgeId invalid_edge_id = 0;
inline constexpr FaceId invalid_face_id = 0;

struct Edge {
    EdgeId id{invalid_edge_id};
    VertexId first{invalid_vertex_id};
    VertexId second{invalid_vertex_id};
};

} // namespace anvil::geometry
