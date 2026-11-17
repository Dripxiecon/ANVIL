#include "mesh_data.hpp"

#include <cstddef>
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
