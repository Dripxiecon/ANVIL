#include "../mesh_data.hpp"

#include <utility>

namespace anvil::geometry {

std::optional<VertexId> MeshData::split_edge(
    EdgeId id,
    const math::Vector3& position
) {
    const auto edge_lookup = edge_slots_.find(id);
    if (edge_lookup == edge_slots_.end() || next_vertex_id_ == invalid_vertex_id
        || next_edge_id_ == invalid_edge_id) {
        return std::nullopt;
    }

    // Store the slot, not the unordered-map iterator: adding the second edge
    // can rehash edge_slots_ and invalidate iterators.
    const std::size_t original_slot = edge_lookup->second;
    const Edge original = edges_[original_slot];
    const auto new_vertex = add_vertex(position);
    if (!new_vertex) {
        return std::nullopt;
    }

    const auto new_edge = add_edge(*new_vertex, original.second);
    if (!new_edge) {
        return std::nullopt;
    }

    // Keep the original edge ID for its first half. Every face that used the
    // original boundary segment receives the new vertex in the matching slot.
    edges_[original_slot].second = *new_vertex;

    for (Face& item : faces_) {
        std::vector<VertexId> updated;
        updated.reserve(item.vertices.size() + 1U);
        for (std::size_t index = 0; index < item.vertices.size(); ++index) {
            const VertexId current = item.vertices[index];
            const VertexId next = item.vertices[(index + 1U) % item.vertices.size()];
            updated.push_back(current);
            const bool follows_original = current == original.first && next == original.second;
            const bool reverses_original = current == original.second && next == original.first;
            if (follows_original || reverses_original) {
                updated.push_back(*new_vertex);
            }
        }
        item.vertices = std::move(updated);
    }

    return new_vertex;
}

} // namespace anvil::geometry
