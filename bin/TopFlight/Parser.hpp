#pragma once

#include <string_view>
#include <string>
#include <stdexcept>
#include <tuple>

#include <external/brigand/brigand.hpp>
#include "external/aggreget/aggreget.hpp"

#include <YoloVM/Instruction.hpp>


namespace TopFlight
{

namespace Parser
{

namespace Impl
{

auto get_next_word(std::string_view str, char delim = ' ') -> std::pair<std::string_view, std::optional<std::string_view>>;
int to_int(std::string_view str);
double to_double(std::string_view str);
std::string extract_string(std::string_view str);
bool to_bool(std::string_view str);
YoloVM::Value parse_and_create_value(std::optional<std::string_view> str);
void check_routine_name(std::string_view routine_name);

} // namespace Impl


struct RoutineCommand
{
	enum class Type
	{
		BEGIN,
		END
	};

	std::string name;
	Type type;
};

bool is_line_a_routine_command(std::string_view line);
std::optional<RoutineCommand> get_routine_command(std::string_view line);

} // namespace Parser

template <std::size_t I, typename InstructionType, typename... Args>
constexpr auto construct_instruction(std::optional<std::string_view> remaining_string, Args&&... args)
{
	if constexpr (I < aggreget::get_tuple_size<InstructionType>())
	{
		if (!remaining_string || remaining_string->empty())
			throw std::runtime_error("Missing argument");

		using AttrType = std::decay_t<decltype(aggreget::get<I>(std::declval<InstructionType>()))>;
		static_assert(std::same_as<AttrType, std::string> || std::same_as<AttrType, YoloVM::Value>);
		if constexpr (std::same_as<AttrType, std::string>)
		{
			auto [arg, remaining_after_arg] = Parser::Impl::get_next_word(*remaining_string);
			remaining_string = remaining_after_arg;
			return construct_instruction<I + 1, InstructionType>(remaining_string, args..., std::string(arg));
		}
		else if constexpr (std::same_as<AttrType, YoloVM::Value>)
		{
			// Works because it is always in last pos but rework needed
			YoloVM::Value v = Parser::Impl::parse_and_create_value(remaining_string);
			return construct_instruction<I + 1, InstructionType>(remaining_string, args..., v);
		}
	}
	else
	{
		return InstructionType{ args... };
	}
}

class InstructionParser
{
	public:
		static YoloVM::Instruction parse(std::string_view str);
};

} // namespace TopFlight
