#pragma once

#include <string_view>
#include <string>
#include <stdexcept>

#include <YoloVM/Instruction.hpp>

namespace Parser
{

namespace Impl
{

inline auto get_next_word(std::string_view str, char delim = ' ') -> std::pair<std::string_view, std::optional<std::string_view>>
{
	auto i = str.find(delim);
	if (i == std::string_view::npos)
		return { str, {} };
	else
		return { str.substr(0, i), str.substr(i + 1)};
}

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

inline int to_int(std::string_view str)
{
	for (auto c: str)
		if (std::isdigit(c) == 0)
			throw std::runtime_error("Invalid integer format");
	int i;
	std::stringstream ss;
	ss << str;
	ss >> i;
	if (ss.bad())
		throw std::runtime_error("Invalid integer format");
	return i;
}

inline double to_double(std::string_view str)
{
	for (auto c: str)
		if (std::isdigit(c) == 0 && c != '.')
			throw std::runtime_error("Invalid integer format");
	double i;
	std::stringstream ss;
	ss << str;
	ss >> i;
	if (ss.bad())
		throw std::runtime_error("Invalid integer format");
	return i;
}

inline std::string extract_string(std::string_view str)
{
	if (str.size() < 2 || str.front() != '"' || str.back() != '"')
		throw std::runtime_error("Invalid string format");

	std::string_view chars = str.substr(1, str.size() - 2);			
	std::string result;
	result.reserve(chars.size());
	std::size_t i = 0;
	while (i < chars.size())
	{
		char c = chars[i];
		if (c != '\\')
		{
			result += c;
		}
		else
		{
			++i;
			if (i >= chars.size())
				throw std::runtime_error("Invalid string format");
			char c2 = chars[i];
			if (c2 == '\\' || c2 == '"')
				result += c2;
			else if (c2 == '0')
				result += '\0';
			else if (c2 == 'b')
				result += '\b';
			else if (c2 == 'n')
				result += '\n';
			else if (c2 == 'r')
				result += '\r';
			else if (c2 == 't')
				result += '\t';
			else if (c2 == 'f')
				result += '\f';
			else if (c2 == 'v')
				result += '\v';
			else
				throw std::runtime_error("Invalid string format unsupported char after \\");
		}
		++i;
	}
	return result;
}

inline bool to_bool(std::string_view str)
{
	if (str == "true")
		return true;
	else if (str == "false")
		return false;
	else
		throw std::runtime_error("Invalid bool parameter");
}

inline Value parse_and_create_value(std::optional<std::string_view> str)
{
	if (!str || str->empty())
		throw std::runtime_error("Missing argument");

	auto [type, remaning_after_type] = get_next_word(*str, '(');
	if (!remaning_after_type || remaning_after_type->empty())
		throw std::runtime_error("Invalid value");

	auto [values, end] = get_next_word(*remaning_after_type, ')');
	if (values.empty() || !end)
		throw std::runtime_error("Invalid value");
	if (!end->empty())
		throw std::runtime_error("Invalid value :  there should not have elements after the ')'");

	if (type == "INTEGER")
	{
		return Integer(to_int(values));
	}
	else if (type == "NUMBER")
	{
		return Number(to_double(values));
	}
	else if (type == "STRING")
	{
		return String(extract_string(values));
	}
	else if (type == "BOOLEAN")
	{
		return Boolean(to_bool(values));
	}
	else if (type == "ARRAY_OF_INTEGER")
	{
		throw std::runtime_error("Not supported yet");
	}
	else if (type == "ARRAY_OF_NUMBER")
	{
		throw std::runtime_error("Not supported yet");
	}
	else if (type == "ARRAY_OF_STRING")
	{
		throw std::runtime_error("Not supported yet");
	}
	else if (type == "ARRAY_OF_BOOLEAN")
	{
		throw std::runtime_error("Not supported yet");
	}
	else
	{
		throw std::runtime_error("Invalid type");
	}
	
	return Integer(5);
}

inline void check_routine_name(std::string_view routine_name)
{
	if (routine_name.empty())
		throw std::runtime_error("Empty routine name is not possible");

	for (char c: routine_name)
	{
		if (std::isalnum(c) == 0 && c != '_')
			throw std::runtime_error("Routine can only contain alphanumerics character or _ : " + std::string(routine_name));
	}
}


} // namespace Impl

Instruction parse_instruction(std::string_view str)
{
    Instruction result;
    auto [instruction, remaining_string] = Impl::get_next_word(str);
    if (instruction == "STORE")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg1, remaning_after_arg1] = Impl::get_next_word(*remaining_string);
        if (!remaning_after_arg1 || remaning_after_arg1->empty())
            throw std::runtime_error("Missing argument");

        result._variant = Instruction::Store{ std::string(arg1), Impl::parse_and_create_value(remaning_after_arg1) };
    }
    else if (instruction == "COPY")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg1, remaning_after_arg1] = Impl::get_next_word(*remaining_string);
        if (!remaning_after_arg1 || remaning_after_arg1->empty())
            throw std::runtime_error("Missing argument");

        auto [arg2, should_be_empty] = Impl::get_next_word(*remaning_after_arg1);
        if (should_be_empty)
            throw std::runtime_error("Too much argument");

        result._variant = Instruction::Copy{ std::string(arg1), std::string(arg2) };
    }
    else if (instruction == "FREE")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg, should_be_empty] = Impl::get_next_word(*remaining_string);
        if (should_be_empty)
            throw std::runtime_error("Too much argument");

        result._variant = Instruction::Free{ std::string(arg) };
    }
    else if (instruction == "ADD")
    {
        result._variant = Impl::parse_and_create_operation<Instruction::Add>(remaining_string);
    }
    else if (instruction == "SUBSTRACT")
    {
        result._variant = Impl::parse_and_create_operation<Instruction::Substract>(remaining_string);
    }
    else if (instruction == "MULTIPLY")
    {
        result._variant = Impl::parse_and_create_operation<Instruction::Multiply>(remaining_string);
    }
    else if (instruction == "DIVIDE")
    {
        result._variant = Impl::parse_and_create_operation<Instruction::Divide>(remaining_string);
    }
    else if (instruction == "MODULO")
    {
        result._variant = Impl::parse_and_create_operation<Instruction::Modulo>(remaining_string);
    }
    else if (instruction == "PRINT")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg, should_be_empty] = Impl::get_next_word(*remaining_string);
        if (should_be_empty)
            throw std::runtime_error("Too much argument");

        result._variant = Instruction::Print{ std::string(arg) };
    }
	else if (instruction == "CALL")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg, should_be_empty] = Impl::get_next_word(*remaining_string);
        if (should_be_empty)
            throw std::runtime_error("Too much argument");

        result._variant = Instruction::Call{ std::string(arg) };
    }
    else
    {
        throw std::runtime_error("Error while parsing unknwown instruction");
    }
    return result;
}

inline bool is_line_a_routine_command(std::string_view line)
{
	return line.front() == '<' && line.back() == '>';
}


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

inline std::optional<RoutineCommand> get_routine_command(std::string_view line)
{
	if (line.size() < 2)
		return {};

	if (line.front() != '<' || line.back() != '>')
		return {};

	if (line[1] == '/')
	{
		RoutineCommand cmd { std::string(line.substr(2, line.size() - 3)), RoutineCommand::Type::END};
		Impl::check_routine_name(cmd.name);
		return cmd;
	}
	else
	{
		RoutineCommand cmd { std::string(line.substr(1, line.size() - 2)), RoutineCommand::Type::BEGIN};
		std::cout << cmd.name << std::endl;
		return cmd;
	}
}

} // namespace Parser
