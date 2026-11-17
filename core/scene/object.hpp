#pragma once

#include "object_id.hpp"
#include "../math/transform.hpp"

#include <optional>
#include <string>
#include <vector>

namespace anvil::scene {

class Scene;

class SceneObject {
public:
    [[nodiscard]] ObjectId id() const noexcept { return id_; }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    void set_name(std::string name) { name_ = std::move(name); }

    [[nodiscard]] const math::Transform& transform() const noexcept { return transform_; }
    [[nodiscard]] math::Transform& transform() noexcept { return transform_; }

    [[nodiscard]] std::optional<ObjectId> parent_id() const noexcept { return parent_id_; }
    [[nodiscard]] const std::vector<ObjectId>& children() const noexcept { return children_; }

private:
    friend class Scene;

    SceneObject(ObjectId id, std::string name)
        : id_(id), name_(std::move(name)) {}

    ObjectId id_{invalid_object_id};
    std::string name_;
    math::Transform transform_{};
    std::optional<ObjectId> parent_id_{};
    std::vector<ObjectId> children_{};
};

} // namespace anvil::scene
