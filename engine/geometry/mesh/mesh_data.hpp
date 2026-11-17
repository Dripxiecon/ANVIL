#pragma once

#include "edge.hpp"
#include "face.hpp"
#include "vertex.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace anvil::geometry {

class MeshData {
public:
    [[nodiscard]] std::optional<VertexId> add_vertex(const math::Vector3& position);
    [[nodiscard]] std::optional<EdgeId> add_edge(VertexId first, VertexId second);
    [[nodiscard]] std::optional<FaceId> add_face(std::vector<VertexId> vertices);
    [[nodiscard]] std::optional<VertexId> split_edge(
        EdgeId id,
        const math::Vector3& position
    );
    [[nodiscard]] bool extrude_face(FaceId id, const math::Vector3& offset);

    [[nodiscard]] bool set_vertex_position(VertexId id, const math::Vector3& position) noexcept;

    [[nodiscard]] const Vertex* vertex(VertexId id) const noexcept;
    [[nodiscard]] Vertex* vertex(VertexId id) noexcept;
    [[nodiscard]] const Edge* edge(EdgeId id) const noexcept;
    [[nodiscard]] const Face* face(FaceId id) const noexcept;

    [[nodiscard]] const std::vector<Vertex>& vertices() const noexcept { return vertices_; }
    [[nodiscard]] const std::vector<Edge>& edges() const noexcept { return edges_; }
    [[nodiscard]] const std::vector<Face>& faces() const noexcept { return faces_; }

    [[nodiscard]] bool validate(std::string* reason = nullptr) const;

    // IDs are deliberately not reset: handles held by other systems must not
    // accidentally refer to newly-created elements after a clear.
    void clear() noexcept;

private:
    [[nodiscard]] bool contains_vertex(VertexId id) const noexcept;
    [[nodiscard]] bool contains_edge(EdgeId id) const noexcept;
    [[nodiscard]] bool contains_face(FaceId id) const noexcept;

    VertexId next_vertex_id_{1};
    EdgeId next_edge_id_{1};
    FaceId next_face_id_{1};

    std::vector<Vertex> vertices_{};
    std::vector<Edge> edges_{};
    std::vector<Face> faces_{};

    std::unordered_map<VertexId, std::size_t> vertex_slots_{};
    std::unordered_map<EdgeId, std::size_t> edge_slots_{};
    std::unordered_map<FaceId, std::size_t> face_slots_{};
};

} // namespace anvil::geometry
