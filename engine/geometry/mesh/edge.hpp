#pragma once

#include <cstdint>

namespace anvil::geometry {

using VertexIndex = std::uint32_t;
using EdgeIndex = std::uint32_t;
using FaceIndex = std::uint32_t;

struct Edge {
    VertexIndex first{0};
    VertexIndex second{0};
};

} // namespace anvil::geometry
