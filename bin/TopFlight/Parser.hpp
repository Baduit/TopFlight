#pragma once

#include <string_view>
#include <string>
#include <stdexcept>

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
YololVM::Value parse_and_create_value(std::optional<std::string_view> str);
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

YololVM::Instruction parse_instruction(std::string_view str);


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

} // namespace TopFlight