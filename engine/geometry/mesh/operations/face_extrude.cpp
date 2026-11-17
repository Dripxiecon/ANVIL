#include "../mesh_data.hpp"

#include <limits>
#include <vector>

namespace anvil::geometry {

bool MeshData::extrude_face(FaceId id, const math::Vector3& offset) {
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

    std::vector<VertexId> extruded;
    extruded.reserve(original.size());
    for (const VertexId old_id : original) {
        const Vertex* old_vertex = vertex(old_id);
        if (old_vertex == nullptr) {
            return false;
        }
        const auto new_id = add_vertex(old_vertex->position + offset);
        if (!new_id) {
            return false;
        }
        extruded.push_back(*new_id);
    }

    // Build the new top boundary and vertical edges before changing the face.
    for (std::size_t index = 0; index < original.size(); ++index) {
        const std::size_t next = (index + 1U) % original.size();
        if (!add_edge(extruded[index], extruded[next])
            || !add_edge(original[index], extruded[index])) {
            return false;
        }
    }

    // Each side quad follows the original face winding. The selected face ID
    // is retained and now represents the translated top polygon.
    for (std::size_t index = 0; index < original.size(); ++index) {
        const std::size_t next = (index + 1U) % original.size();
        if (!add_face({
                original[index],
                original[next],
                extruded[next],
                extruded[index]
            })) {
            return false;
        }
    }

    Face* top = nullptr;
    const auto face_slot = face_slots_.find(id);
    if (face_slot != face_slots_.end()) {
        top = &faces_[face_slot->second];
    }
    if (top == nullptr) {
        return false;
    }
    top->vertices = std::move(extruded);
    return true;
}

} // namespace anvil::geometry
