#pragma once

#include <cstdint>

namespace anvil::scene {

using ObjectId = std::uint64_t;
using CollectionId = std::uint64_t;

inline constexpr ObjectId invalid_object_id = 0;
inline constexpr CollectionId invalid_collection_id = 0;

} // namespace anvil::scene
