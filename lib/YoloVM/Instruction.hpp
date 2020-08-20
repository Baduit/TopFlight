#pragma once

#include <string>
#include <variant>
#include <string_view>
#include <stdexcept>
#include <optional>
#include <utility>
#include <vector>
#include <cctype>
#include <charconv>
#include <sstream>

#include "Value.hpp"



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

	template <typename F>
	auto visit(F&& f) const
	{
		return std::visit(std::forward<F>(f), _variant);
	}

	Variant _variant;
};