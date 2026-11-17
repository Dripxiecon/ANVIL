#pragma once

#include "object_id.hpp"

#include <string>
#include <utility>
#include <unordered_set>

namespace anvil::scene {

class Scene;

class Collection {
public:
    [[nodiscard]] CollectionId id() const noexcept { return id_; }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    void set_name(std::string name) { name_ = std::move(name); }
    [[nodiscard]] const std::unordered_set<ObjectId>& object_ids() const noexcept {
        return object_ids_;
    }

private:
    friend class Scene;

    Collection(CollectionId id, std::string name)
        : id_(id), name_(std::move(name)) {}

    CollectionId id_{invalid_collection_id};
    std::string name_;
    std::unordered_set<ObjectId> object_ids_{};
};

} // namespace anvil::scene
