#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <iostream>

struct Integer
{
	constexpr Integer(int v):
		value(v)
	{}

	constexpr Integer() = default;

	constexpr Integer(const Integer&) = default;
	constexpr Integer& operator=(const Integer&) = default;

	constexpr Integer(Integer&&) = default;
	constexpr Integer& operator=(Integer&&) = default;

	friend constexpr Integer operator+(Integer a, Integer b) { return Integer{ a.value + b.value }; }
	friend constexpr Integer operator-(Integer a, Integer b) { return Integer{ a.value - b.value }; }
	friend constexpr Integer operator*(Integer a, Integer b) { return Integer{ a.value * b.value }; }
	friend constexpr Integer operator/(Integer a, Integer b) { return Integer{ a.value / b.value }; }
	friend constexpr Integer operator%(Integer a, Integer b) { return Integer{ a.value % b.value }; }

	void print(std::ostream& out) const
	{
		out << value;
	}

	int value = 0;
};

struct Number
{
	constexpr Number(double v):
		value(v)
	{}

	constexpr Number() = default;

	constexpr Number(const Number&) = default;
	constexpr Number& operator=(const Number&) = default;

	constexpr Number(Number&&) = default;
	constexpr Number& operator=(Number&&) = default;

	friend constexpr Number operator+(Number a, Number b) { return Number{ a.value + b.value }; }
	friend constexpr Number operator-(Number a, Number b) { return Number{ a.value - b.value }; }
	friend constexpr Number operator*(Number a, Number b) { return Number{ a.value * b.value }; }
	friend constexpr Number operator/(Number a, Number b) { return Number{ a.value / b.value }; }
	// No modulo

	void print(std::ostream& out) const
	{
		out << value;
	}

	double value = 0.0;
};

struct String
{
	String(std::string v):
		value(std::move(v))
	{}

	String() = default;

	String(const String&) = default;
	String& operator=(const String&) = default;

	String(String&&) = default;
	String& operator=(String&&) = default;

	void print(std::ostream& out) const
	{
		out << value;
	}

	std::string value;
};


template <typename T>
struct ArrayOf
{
	ArrayOf(std::vector<T> v):
		values(std::move(v))
	{}

	ArrayOf() = default;

	ArrayOf(const ArrayOf&) = default;
	ArrayOf& operator=(const ArrayOf&) = default;

	ArrayOf(ArrayOf&&) = default;
	ArrayOf& operator=(ArrayOf&&) = default;

	void print(std::ostream& out) const
	{
		out << '[';
		for (auto it = values.cbegin(); it != values.cend(); ++it)
		{
			it->print(out);
			if (it + 1 != values.cend())
				out << ", ";
		}
		out << ']';
	}

	std::vector<T> values;
};

using ArrayOfInteger = ArrayOf<Integer>;
using ArrayOfNumber = ArrayOf<Number>;
using ArrayOfString = ArrayOf<String>;

