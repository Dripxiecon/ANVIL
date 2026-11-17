#include "../../../../core/scene/scene.hpp"

#include <cassert>
#include <optional>

using anvil::scene::Scene;

int main() {
    Scene scene;
    const auto root = scene.create_object("Root");
    const auto child = scene.create_object("Child");
    const auto grandchild = scene.create_object("Grandchild");

    assert(scene.reparent_object(child, root));
    assert(scene.reparent_object(grandchild, child));
    assert(!scene.reparent_object(root, grandchild));
    assert(!scene.reparent_object(child, 9999));
    assert(scene.find_object(root)->children().size() == 1U);
    assert(scene.find_object(child)->parent_id() == root);

    assert(scene.reparent_object(child, std::nullopt));
    assert(scene.find_object(root)->children().empty());
    assert(!scene.find_object(child)->parent_id());

    const auto collection = scene.create_collection("Geometry");
    assert(scene.add_to_collection(collection, root));
    assert(!scene.add_to_collection(collection, root));
    assert(!scene.add_to_collection(collection, 9999));
    assert(scene.find_collection(collection)->object_ids().contains(root));

    assert(scene.remove_object(root));
    assert(!scene.find_object(root));
    assert(scene.find_collection(collection)->object_ids().empty());
    assert(scene.remove_object(child));
    assert(scene.remove_object(grandchild));
    assert(!scene.remove_object(9999));
    assert(scene.remove_collection(collection));
    assert(!scene.remove_collection(collection));
}
