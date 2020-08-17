#pragma once

#include <string>
#include <variant>
#include <string_view>
#include <stdexcept>
#include <optional>
#include <utility>

#include "Value.hpp"

auto get_next_word(std::string_view str) -> std::pair<std::string_view, std::optional<std::string_view>>
{
	auto i = str.find(' ');
	if (i == std::string_view::npos)
		return { str, {} };
	else
		return { str.substr(0, i), str.substr(i + 1)};
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

		}
		else if (instruction == "COPY")
		{

		}
		else if (instruction == "FREE")
		{

		}
		else if (instruction == "ADD")
		{

		}
		else if (instruction == "SUBSTRACT")
		{

		}
		else if (instruction == "MULTIPLY")
		{

		}
		else if (instruction == "DIVIDE")
		{

		}
		else if (instruction == "MODULO")
		{

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