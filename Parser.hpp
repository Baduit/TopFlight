#pragma once

#include <string_view>
#include <string>

#include "Instruction.hpp"

namespace Parser
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
	else if (type == "ARRAY_OF_INTEGER")
	{

	}
	else if (type == "ARRAY_OF_NUMBER")
	{

	}
	else if (type == "ARRAY_OF_STRING")
	{

	}
	else
	{
		throw std::runtime_error("Invalid type");
	}
	
	return Integer(5);
}

Instruction parse_instruction(std::string_view str)
{
    Instruction result;
    auto [instruction, remaining_string] = get_next_word(str);
    if (instruction == "STORE")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg1, remaning_after_arg1] = get_next_word(*remaining_string);
        if (!remaning_after_arg1 || remaning_after_arg1->empty())
            throw std::runtime_error("Missing argument");

        result._variant = Instruction::Store{ std::string(arg1), parse_and_create_value(remaning_after_arg1) };
    }
    else if (instruction == "COPY")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg1, remaning_after_arg1] = get_next_word(*remaining_string);
        if (!remaning_after_arg1 || remaning_after_arg1->empty())
            throw std::runtime_error("Missing argument");

        auto [arg2, should_be_empty] = get_next_word(*remaning_after_arg1);
        if (should_be_empty)
            throw std::runtime_error("Too much argument");

        result._variant = Instruction::Copy{ std::string(arg1), std::string(arg2) };
    }
    else if (instruction == "FREE")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg, should_be_empty] = get_next_word(*remaining_string);
        if (should_be_empty)
            throw std::runtime_error("Too much argument");

        result._variant = Instruction::Free{ std::string(arg) };
    }
    else if (instruction == "ADD")
    {
        result._variant = parse_and_create_operation<Instruction::Add>(remaining_string);
    }
    else if (instruction == "SUBSTRACT")
    {
        result._variant = parse_and_create_operation<Instruction::Substract>(remaining_string);
    }
    else if (instruction == "MULTIPLY")
    {
        result._variant = parse_and_create_operation<Instruction::Multiply>(remaining_string);
    }
    else if (instruction == "DIVIDE")
    {
        result._variant = parse_and_create_operation<Instruction::Divide>(remaining_string);
    }
    else if (instruction == "MODULO")
    {
        result._variant = parse_and_create_operation<Instruction::Modulo>(remaining_string);
    }
    else if (instruction == "PRINT")
    {
        if (!remaining_string || remaining_string->empty())
            throw std::runtime_error("Missing argument");

        auto [arg, should_be_empty] = get_next_word(*remaining_string);
        if (should_be_empty)
            throw std::runtime_error("Too much argument");

        result._variant = Instruction::Print{ std::string(arg) };
    }
    else
    {
        throw std::runtime_error("Error while parsing unknwown instruction");
    }
    return result;
}

} // namespace Parser
