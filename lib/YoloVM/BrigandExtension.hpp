#pragma once

#include <variant>

#define BRIGAND_NO_BOOST_SUPPORT
#include <brigand/brigand.hpp>

namespace YoloVM
{
	template <typename... T>
	using variant_wrapper = typename std::variant<T...>;

	template <typename L>
	using brigand_list_as_variant = brigand::wrap<L, variant_wrapper>;
} // namespace YoloVM