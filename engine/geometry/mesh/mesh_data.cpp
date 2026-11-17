#include "mesh_data.hpp"

#include <algorithm>
#include <limits>
#include <unordered_set>
#include <utility>

namespace anvil::geometry {
namespace {

template <typename T>
bool can_append(const std::vector<T>& values) {
    return values.size() < static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max());
}

bool contains_duplicate(const std::vector<VertexIndex>& indices) {
    std::unordered_set<VertexIndex> seen;
    seen.reserve(indices.size());
    for (const VertexIndex index : indices) {
        if (!seen.insert(index).second) {
            return true;
        }
    }
    return false;
}

} // namespace

std::optional<VertexIndex> MeshData::add_vertex(const math::Vector3& position) {
    if (!can_append(vertices_)) {
        return std::nullopt;
    }

    const auto index = static_cast<VertexIndex>(vertices_.size());
    vertices_.push_back(Vertex{position});
    return index;
}

std::optional<EdgeIndex> MeshData::add_edge(VertexIndex first, VertexIndex second) {
    if (first >= vertices_.size() || second >= vertices_.size() || first == second
        || !can_append(edges_)) {
        return std::nullopt;
    }

    const auto index = static_cast<EdgeIndex>(edges_.size());
    edges_.push_back(Edge{first, second});
    return index;
}

std::optional<FaceIndex> MeshData::add_face(std::vector<VertexIndex> vertices) {
    if (vertices.size() < 3U || !can_append(faces_) || contains_duplicate(vertices)) {
        return std::nullopt;
    }

    for (const VertexIndex index : vertices) {
        if (index >= vertices_.size()) {
            return std::nullopt;
        }
    }

    const auto index = static_cast<FaceIndex>(faces_.size());
    faces_.push_back(Face{std::move(vertices)});
    return index;
}

const Vertex* MeshData::vertex(VertexIndex index) const noexcept {
    return index < vertices_.size() ? &vertices_[index] : nullptr;
}

const Edge* MeshData::edge(EdgeIndex index) const noexcept {
    return index < edges_.size() ? &edges_[index] : nullptr;
}

const Face* MeshData::face(FaceIndex index) const noexcept {
    return index < faces_.size() ? &faces_[index] : nullptr;
}

bool MeshData::validate(std::string* reason) const {
    const auto fail = [reason](const char* message) {
        if (reason != nullptr) {
            *reason = message;
        }
        return false;
    };

    for (const Edge& item : edges_) {
        if (item.first >= vertices_.size() || item.second >= vertices_.size()
            || item.first == item.second) {
            return fail("Edge references an invalid or identical vertex pair");
        }
    }

    for (const Face& item : faces_) {
        if (item.vertices.size() < 3U || contains_duplicate(item.vertices)) {
            return fail("Face has fewer than three vertices or repeats a vertex");
        }
        for (const VertexIndex index : item.vertices) {
            if (index >= vertices_.size()) {
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
}

} // namespace anvil::geometry
