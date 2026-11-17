#include "scene.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <utility>

namespace anvil::scene {

ObjectId Scene::create_object(std::string name) {
    if (next_object_id_ == invalid_object_id) {
        throw std::overflow_error("Scene object ID space exhausted");
    }

    const ObjectId id = next_object_id_++;
    objects_.emplace(id, SceneObject{id, std::move(name)});
    return id;
}

bool Scene::remove_object(ObjectId id) {
    const auto found = objects_.find(id);
    if (found == objects_.end()) {
        return false;
    }

    SceneObject& object = found->second;
    if (object.parent_id_) {
        if (SceneObject* parent = find_object(*object.parent_id_)) {
            auto& siblings = parent->children_;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), id), siblings.end());
        }
    }

    for (const ObjectId child_id : object.children_) {
        if (SceneObject* child = find_object(child_id)) {
            child->parent_id_.reset();
        }
    }

    for (auto& [collection_id, collection] : collections_) {
        (void)collection_id;
        collection.object_ids_.erase(id);
    }

    objects_.erase(found);
    return true;
}

bool Scene::reparent_object(ObjectId child_id, std::optional<ObjectId> parent_id) {
    SceneObject* child = find_object(child_id);
    if (child == nullptr) {
        return false;
    }

    if (parent_id && (find_object(*parent_id) == nullptr
        || *parent_id == child_id
        || would_create_cycle(child_id, *parent_id))) {
        return false;
    }

    if (child->parent_id_ == parent_id) {
        return true;
    }

    if (child->parent_id_) {
        if (SceneObject* old_parent = find_object(*child->parent_id_)) {
            auto& old_children = old_parent->children_;
            old_children.erase(
                std::remove(old_children.begin(), old_children.end(), child_id),
                old_children.end()
            );
        }
    }

    child->parent_id_ = parent_id;
    if (parent_id) {
        find_object(*parent_id)->children_.push_back(child_id);
    }
    return true;
}

CollectionId Scene::create_collection(std::string name) {
    if (next_collection_id_ == invalid_collection_id) {
        throw std::overflow_error("Scene collection ID space exhausted");
    }

    const CollectionId id = next_collection_id_++;
    collections_.emplace(id, Collection{id, std::move(name)});
    return id;
}

bool Scene::remove_collection(CollectionId id) {
    return collections_.erase(id) != 0U;
}

bool Scene::add_to_collection(CollectionId collection_id, ObjectId object_id) {
    Collection* collection = find_collection(collection_id);
    if (collection == nullptr || find_object(object_id) == nullptr) {
        return false;
    }
    return collection->object_ids_.insert(object_id).second;
}

bool Scene::remove_from_collection(CollectionId collection_id, ObjectId object_id) {
    Collection* collection = find_collection(collection_id);
    return collection != nullptr && collection->object_ids_.erase(object_id) != 0U;
}

SceneObject* Scene::find_object(ObjectId id) noexcept {
    const auto found = objects_.find(id);
    return found == objects_.end() ? nullptr : &found->second;
}

const SceneObject* Scene::find_object(ObjectId id) const noexcept {
    const auto found = objects_.find(id);
    return found == objects_.end() ? nullptr : &found->second;
}

Collection* Scene::find_collection(CollectionId id) noexcept {
    const auto found = collections_.find(id);
    return found == collections_.end() ? nullptr : &found->second;
}

const Collection* Scene::find_collection(CollectionId id) const noexcept {
    const auto found = collections_.find(id);
    return found == collections_.end() ? nullptr : &found->second;
}

bool Scene::would_create_cycle(ObjectId child_id, ObjectId parent_id) const noexcept {
    const SceneObject* current = find_object(parent_id);
    while (current != nullptr) {
        if (current->id() == child_id) {
            return true;
        }
        if (!current->parent_id()) {
            return false;
        }
        current = find_object(*current->parent_id());
    }
    return false;
}

} // namespace anvil::scene
