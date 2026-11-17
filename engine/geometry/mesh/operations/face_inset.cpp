#include "../mesh_data.hpp"

#include <cmath>
#include <limits>
#include <vector>

namespace anvil::geometry {

bool MeshData::inset_face(FaceId id, float factor) {
    if (!std::isfinite(factor) || factor <= 0.0F || factor >= 1.0F) {
        return false;
    }

    const Face* selected = face(id);
    if (selected == nullptr || selected->vertices.size() < 3U) {
        return false;
    }

    const std::vector<VertexId> original = selected->vertices;
    const std::uint64_t count = static_cast<std::uint64_t>(original.size());
    const auto can_allocate = [](std::uint64_t next, std::uint64_t amount) {
        return next != 0U && amount > 0U
            && amount - 1U <= std::numeric_limits<std::uint64_t>::max() - next;
    };
    if (!can_allocate(next_vertex_id_, count)
        || !can_allocate(next_edge_id_, count * 2U)
        || !can_allocate(next_face_id_, count)) {
        return false;
    }

    math::Vector3 centroid{};
    for (const VertexId vertex_id : original) {
        const Vertex* item = vertex(vertex_id);
        if (item == nullptr) {
            return false;
        }
        centroid += item->position;
    }
    centroid = centroid / static_cast<float>(original.size());

    std::vector<VertexId> inset_vertices;
    inset_vertices.reserve(original.size());
    for (const VertexId vertex_id : original) {
        const Vertex* item = vertex(vertex_id);
        const math::Vector3 inset_position =
            item->position + (centroid - item->position) * factor;
        const auto new_id = add_vertex(inset_position);
        if (!new_id) {
            return false;
        }
        inset_vertices.push_back(*new_id);
    }

    for (std::size_t index = 0; index < original.size(); ++index) {
        const std::size_t next = (index + 1U) % original.size();
        if (!add_edge(inset_vertices[index], inset_vertices[next])
            || !add_edge(original[index], inset_vertices[index])) {
            return false;
        }
    }

    for (std::size_t index = 0; index < original.size(); ++index) {
        const std::size_t next = (index + 1U) % original.size();
        if (!add_face({
                original[index],
                original[next],
                inset_vertices[next],
                inset_vertices[index]
            })) {
            return false;
        }
    }

    Face* inner = nullptr;
    const auto face_slot = face_slots_.find(id);
    if (face_slot != face_slots_.end()) {
        inner = &faces_[face_slot->second];
    }
    if (inner == nullptr) {
        return false;
    }
    inner->vertices = std::move(inset_vertices);
    return true;
}

} // namespace anvil::geometry
