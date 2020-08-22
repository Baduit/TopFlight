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

template <typename Operation>
Operation parse_and_create_operation(std::optional<std::string_view> str)
{
	if (!str || str->empty())
		throw std::runtime_error("Missing argument");

	auto [arg1, remaning_after_arg1] = get_next_word(*str);
	if (!remaning_after_arg1 || remaning_after_arg1->empty())
		throw std::runtime_error("Missing argument");

	auto [arg2, remaning_after_arg2] = get_next_word(*remaning_after_arg1);
	if (!remaning_after_arg2 || remaning_after_arg2->empty())
		throw std::runtime_error("Missing argument");

	auto [arg3, should_be_empty] = get_next_word(*remaning_after_arg2);
	if (should_be_empty)
		throw std::runtime_error("Too much argument");

	return Operation{ std::string(arg1), std::string(arg2), std::string(arg3) };
}



} // namespace Impl

YoloVM::Instruction parse_instruction(std::string_view str);


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
			std::string s = std::string(arg);
			return construct_instruction<I + 1, InstructionType>(remaining_string, args..., s);
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
		static YoloVM::Instruction parse(std::string_view str)
		{
			std::optional<YoloVM::Instruction> result;
    		auto [instruction, remaining_string] = Parser::Impl::get_next_word(str);

			brigand::for_each<YoloVM::Instruction::InstructionTypes>(
				[&](auto type)
				{
					using InstructionType = decltype(type)::type;
					constexpr auto type_name = InstructionType::NAME;
					if (type_name == instruction)
					{
						result = construct_instruction<0, InstructionType>(remaining_string);
					}
				});
			if (!result)
				throw std::runtime_error("Error while parsing unknwown instruction");

			// Check for no extra arguments
			return *result;
		}
};

} // namespace TopFlight
