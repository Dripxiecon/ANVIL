#pragma once

#include "collection.hpp"
#include "object.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace anvil::scene {

class Scene {
public:
    Scene() = default;

    [[nodiscard]] ObjectId create_object(std::string name);
    [[nodiscard]] bool remove_object(ObjectId id);
    [[nodiscard]] bool reparent_object(ObjectId child_id, std::optional<ObjectId> parent_id);

    [[nodiscard]] CollectionId create_collection(std::string name);
    [[nodiscard]] bool remove_collection(CollectionId id);
    [[nodiscard]] bool add_to_collection(CollectionId collection_id, ObjectId object_id);
    [[nodiscard]] bool remove_from_collection(CollectionId collection_id, ObjectId object_id);

    [[nodiscard]] SceneObject* find_object(ObjectId id) noexcept;
    [[nodiscard]] const SceneObject* find_object(ObjectId id) const noexcept;
    [[nodiscard]] Collection* find_collection(CollectionId id) noexcept;
    [[nodiscard]] const Collection* find_collection(CollectionId id) const noexcept;

    [[nodiscard]] const std::unordered_map<ObjectId, SceneObject>& objects() const noexcept {
        return objects_;
    }
    [[nodiscard]] const std::unordered_map<CollectionId, Collection>& collections() const noexcept {
        return collections_;
    }

private:
    [[nodiscard]] bool would_create_cycle(ObjectId child_id, ObjectId parent_id) const noexcept;

    ObjectId next_object_id_{1};
    CollectionId next_collection_id_{1};
    std::unordered_map<ObjectId, SceneObject> objects_{};
    std::unordered_map<CollectionId, Collection> collections_{};
};

} // namespace anvil::scene
