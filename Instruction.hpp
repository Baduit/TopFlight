#pragma once

#include <string>
#include <variant>
#include <string_view>
#include <stdexcept>
#include <optional>
#include <utility>
#include <vector>

#include "Value.hpp"

inline auto get_next_word(std::string_view str) -> std::pair<std::string_view, std::optional<std::string_view>>
{
	auto i = str.find(' ');
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

inline Value parse_and_create_value(std::optional<std::string_view> str)
{
	if (!str || str->empty())
		throw std::runtime_error("Missing argument");
	return Integer(5);
}

struct Instruction
{
	struct Store
	{
		std::string dest;
		Value value;
	};

	struct Copy
	{
		std::string input;
		std::string dest;
	};

	struct Free
	{
		std::string dest;
	};

	struct Add
	{
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Substract
	{
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Multiply
	{
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Divide
	{
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Modulo
	{
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Print
	{
		std::string input;
	};

	using Variant = std::variant<Store, Copy, Free, Add, Substract, Multiply, Divide, Modulo, Print>;

	template <typename T>
	Instruction(T i):
		_variant(std::move(i))
	{}

	Instruction() = default;

	Instruction(const Instruction&) = default;
	Instruction& operator=(const Instruction&) = default;

	Instruction(Instruction&&) = default;
	Instruction& operator=(Instruction&&) = default;

	static Instruction from_string(std::string_view str)
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

			result._variant = Store{ std::string(arg1), parse_and_create_value(remaning_after_arg1) };
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

			result._variant = Copy{ std::string(arg1), std::string(arg2) };
		}
		else if (instruction == "FREE")
		{
			if (!remaining_string || remaining_string->empty())
				throw std::runtime_error("Missing argument");

			auto [arg, should_be_empty] = get_next_word(*remaining_string);
			if (should_be_empty)
				throw std::runtime_error("Too much argument");

			result._variant = Free{ std::string(arg) };
		}
		else if (instruction == "ADD")
		{
			result._variant = parse_and_create_operation<Add>(remaining_string);
		}
		else if (instruction == "SUBSTRACT")
		{
			result._variant = parse_and_create_operation<Substract>(remaining_string);
		}
		else if (instruction == "MULTIPLY")
		{
			result._variant = parse_and_create_operation<Multiply>(remaining_string);
		}
		else if (instruction == "DIVIDE")
		{
			result._variant = parse_and_create_operation<Divide>(remaining_string);
		}
		else if (instruction == "MODULO")
		{
			result._variant = parse_and_create_operation<Modulo>(remaining_string);
		}
		else if (instruction == "PRINT")
		{
			if (!remaining_string || remaining_string->empty())
				throw std::runtime_error("Missing argument");

			auto [arg, should_be_empty] = get_next_word(*remaining_string);
			if (should_be_empty)
				throw std::runtime_error("Too much argument");

			result._variant = Print{ std::string(arg) };
		}
		else
		{
			throw std::runtime_error("Error while parsing unknwown instruction");
		}
		return result;
	}

	template <typename F>
	auto visit(F&& f) const
	{
		return std::visit(std::forward<F>(f), _variant);
	}

	Variant _variant;
};