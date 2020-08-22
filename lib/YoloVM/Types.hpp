#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <compare>

namespace YoloVM
{

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

	friend constexpr bool operator==(Integer a, Integer b) = default;
	friend constexpr std::strong_ordering operator<=>(Integer a, Integer b) = default;

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

	friend constexpr bool operator==(Number a, Number b) = default;
	friend constexpr std::strong_ordering operator<=>(Number a, Number b) = default;

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

	friend bool operator==(String a, String b) = default;
	friend std::strong_ordering operator<=>(String a, String b) = default;

	void print(std::ostream& out) const
	{
		out << value;
	}

	std::string value;
};

struct Boolean
{
	constexpr Boolean(bool v):
		value(std::move(v))
	{}

	constexpr Boolean() = default;

	constexpr Boolean(const Boolean&) = default;
	constexpr Boolean& operator=(const Boolean&) = default;

	constexpr Boolean(Boolean&&) = default;
	constexpr Boolean& operator=(Boolean&&) = default;

	void print(std::ostream& out) const
	{
		out << (value ? "true" : "false");
	}

	friend constexpr bool operator==(Boolean a, Boolean b) = default;
	friend constexpr std::strong_ordering operator<=>(Boolean a, Boolean b) = default;

	friend constexpr Boolean operator&&(Boolean a, Boolean b) { return a.value && b.value; }
	friend constexpr Boolean operator||(Boolean a, Boolean b) { return a.value && b.value; }
	constexpr Boolean operator!() const { return Boolean(!value); }

	bool value = true;
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

	friend bool operator==(ArrayOf a, ArrayOf b) = default;
	friend std::strong_ordering operator<=>(ArrayOf a, ArrayOf b) = default;

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
using ArrayOfBoolean = ArrayOf<Boolean>;

} // YoloVM