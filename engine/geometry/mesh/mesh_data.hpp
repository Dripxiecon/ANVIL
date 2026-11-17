#pragma once

#include "edge.hpp"
#include "face.hpp"
#include "vertex.hpp"

#include <optional>
#include <string>
#include <vector>

namespace anvil::geometry {

class MeshData {
public:
    [[nodiscard]] std::optional<VertexIndex> add_vertex(const math::Vector3& position);
    [[nodiscard]] std::optional<EdgeIndex> add_edge(VertexIndex first, VertexIndex second);
    [[nodiscard]] std::optional<FaceIndex> add_face(std::vector<VertexIndex> vertices);

    [[nodiscard]] const Vertex* vertex(VertexIndex index) const noexcept;
    [[nodiscard]] const Edge* edge(EdgeIndex index) const noexcept;
    [[nodiscard]] const Face* face(FaceIndex index) const noexcept;

    [[nodiscard]] const std::vector<Vertex>& vertices() const noexcept { return vertices_; }
    [[nodiscard]] const std::vector<Edge>& edges() const noexcept { return edges_; }
    [[nodiscard]] const std::vector<Face>& faces() const noexcept { return faces_; }

    [[nodiscard]] bool validate(std::string* reason = nullptr) const;

    void clear() noexcept;

private:
    std::vector<Vertex> vertices_{};
    std::vector<Edge> edges_{};
    std::vector<Face> faces_{};
};

} // namespace anvil::geometry
