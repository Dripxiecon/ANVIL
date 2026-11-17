#include "mesh_data.hpp"

#include <algorithm>
#include <limits>
#include <unordered_set>
#include <utility>

namespace anvil::geometry {
namespace {

template <typename Id>
bool contains_duplicate(const std::vector<Id>& ids) {
    std::unordered_set<Id> seen;
    seen.reserve(ids.size());
    for (const Id id : ids) {
        if (!seen.insert(id).second) {
            return true;
        }
    }
    return false;
}

} // namespace

std::optional<VertexId> MeshData::add_vertex(const math::Vector3& position) {
    if (next_vertex_id_ == invalid_vertex_id) {
        return std::nullopt;
    }

    const VertexId id = next_vertex_id_++;
    const std::size_t slot = vertices_.size();
    vertices_.push_back(Vertex{id, position});
    try {
        vertex_slots_.emplace(id, slot);
    } catch (...) {
        vertices_.pop_back();
        throw;
    }
    return id;
}

std::optional<EdgeId> MeshData::add_edge(VertexId first, VertexId second) {
    if (!contains_vertex(first) || !contains_vertex(second) || first == second
        || next_edge_id_ == invalid_edge_id) {
        return std::nullopt;
    }

    const EdgeId id = next_edge_id_++;
    const std::size_t slot = edges_.size();
    edges_.push_back(Edge{id, first, second});
    try {
        edge_slots_.emplace(id, slot);
    } catch (...) {
        edges_.pop_back();
        throw;
    }
    return id;
}

std::optional<FaceId> MeshData::add_face(std::vector<VertexId> vertices) {
    if (vertices.size() < 3U || next_face_id_ == invalid_face_id
        || contains_duplicate(vertices)) {
        return std::nullopt;
    }

    for (const VertexId id : vertices) {
        if (!contains_vertex(id)) {
            return std::nullopt;
        }
    }

    const FaceId id = next_face_id_++;
    const std::size_t slot = faces_.size();
    faces_.push_back(Face{id, std::move(vertices)});
    try {
        face_slots_.emplace(id, slot);
    } catch (...) {
        faces_.pop_back();
        throw;
    }
    return id;
}

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

bool MeshData::set_vertex_position(
    VertexId id,
    const math::Vector3& position
) noexcept {
    Vertex* item = vertex(id);
    if (item == nullptr) {
        return false;
    }
    item->position = position;
    return true;
}

const Vertex* MeshData::vertex(VertexId id) const noexcept {
    const auto found = vertex_slots_.find(id);
    return found == vertex_slots_.end() ? nullptr : &vertices_[found->second];
}

Vertex* MeshData::vertex(VertexId id) noexcept {
    const auto found = vertex_slots_.find(id);
    return found == vertex_slots_.end() ? nullptr : &vertices_[found->second];
}

const Edge* MeshData::edge(EdgeId id) const noexcept {
    const auto found = edge_slots_.find(id);
    return found == edge_slots_.end() ? nullptr : &edges_[found->second];
}

const Face* MeshData::face(FaceId id) const noexcept {
    const auto found = face_slots_.find(id);
    return found == face_slots_.end() ? nullptr : &faces_[found->second];
}

bool MeshData::validate(std::string* reason) const {
    const auto fail = [reason](const char* message) {
        if (reason != nullptr) {
            *reason = message;
        }
        return false;
    };

    for (const Vertex& item : vertices_) {
        if (item.id == invalid_vertex_id || !contains_vertex(item.id)) {
            return fail("Vertex has an invalid stable ID");
        }
    }

    for (const Edge& item : edges_) {
        if (item.id == invalid_edge_id || !contains_edge(item.id)
            || !contains_vertex(item.first) || !contains_vertex(item.second)
            || item.first == item.second) {
            return fail("Edge contains an invalid ID or vertex reference");
        }
    }

    for (const Face& item : faces_) {
        if (item.id == invalid_face_id || !contains_face(item.id)
            || item.vertices.size() < 3U || contains_duplicate(item.vertices)) {
            return fail("Face has an invalid ID or repeats/fails to provide vertices");
        }
        for (const VertexId id : item.vertices) {
            if (!contains_vertex(id)) {
                return fail("Face references a vertex that does not exist");
            }
        }
    }

    if (reason != nullptr) {
        reason->clear();
    }
    return true;
}

void MeshData::clear() noexcept {
    vertices_.clear();
    edges_.clear();
    faces_.clear();
    vertex_slots_.clear();
    edge_slots_.clear();
    face_slots_.clear();
}

bool MeshData::contains_vertex(VertexId id) const noexcept {
    return vertex_slots_.contains(id);
}

bool MeshData::contains_edge(EdgeId id) const noexcept {
    return edge_slots_.contains(id);
}

bool MeshData::contains_face(FaceId id) const noexcept {
    return face_slots_.contains(id);
}

} // namespace anvil::geometry
