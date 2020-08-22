#pragma once

#include <type_traits>

namespace aggreget
{

template <typename T>
concept Aggregate = std::is_aggregate_v<T>;

}