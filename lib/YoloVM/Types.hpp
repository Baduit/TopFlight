#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <compare>
#include <string_view>

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

	friend constexpr bool operator==(Number a, Number b)
	{
		return (a.value + 0.0001 >= b.value) && (a.value - 0.0001 <= b.value);
	}

	friend constexpr std::strong_ordering operator<=>(Number a, Number b)
	{
		if (a == b)
			return std::strong_ordering::equal;
		else if (a.value > b.value)
			return std::strong_ordering::less;
		else
			return std::strong_ordering::greater;
	}

	void print(std::ostream& out) const
	{
		out << value;
	}

	double value = 0.0;
};

struct Char
{
	constexpr Char(char v):
		value(std::move(v))
	{}

	constexpr Char() = default;

	constexpr Char(const Char&) = default;
	constexpr Char& operator=(const Char&) = default;

	constexpr Char(Char&&) = default;
	constexpr Char& operator=(Char&&) = default;

	void print(std::ostream& out) const
	{
		out << value;
	}

	friend constexpr bool operator==(Char a, Char b) = default;
	friend constexpr std::strong_ordering operator<=>(Char a, Char b) = default;

	char value = '\0';
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

	Char get_at(Integer index) const
	{
		if (index.value < 0)
			throw std::out_of_range("Index can't be negative");
		return value.at(static_cast<std::size_t>(index.value));
	}

	void store_at(Integer index, Char c)
	{
		if (index.value < 0)
			throw std::out_of_range("Index can't be negative");
		value.at(static_cast<std::size_t>(index.value)) = c.value;
	}

	Integer size() const
	{
		return Integer(static_cast<int>(value.size()));
	}

	void resize(Integer new_size)
	{
		if (new_size.value < 0)
			throw std::out_of_range("New size can't be negative");
		value.resize(static_cast<std::size_t>(new_size.value));
	}

	void insert(Integer index, Char input)
	{
		if (index.value < 0)
			throw std::out_of_range("Index can't be negative");
		if (static_cast<std::size_t>(index.value) > value.size())
			throw std::out_of_range("Index is out of range");
		value.insert(value.begin() + index.value, input.value);
	}

	void push_back(Char input)
	{
		value.push_back(input.value);
	}

	String concat(const String& other) const
	{
		String new_array = (*this);
		new_array.value.insert(new_array.value.end(), other.value.cbegin(), other.value.cend());
		return new_array;
	}

	void erase(Integer index)
	{
		value.erase(value.begin() + index.value);
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
	using ElementType = T;

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

	const T& get_at(Integer index) const
	{
		if (index.value < 0)
			throw std::out_of_range("Index can't be negative");
		return values.at(static_cast<std::size_t>(index.value));
	}

	void store_at(Integer index, T value)
	{
		if (index.value < 0)
			throw std::out_of_range("Index can't be negative");
		values.at(static_cast<std::size_t>(index.value)) = std::move(value);
	}

	Integer size() const
	{
		return Integer(static_cast<int>(values.size()));
	}

	void resize(Integer new_size)
	{
		if (new_size.value < 0)
			throw std::out_of_range("New size can't be negative");
		values.resize(static_cast<std::size_t>(new_size.value));
	}

	void insert(Integer index, const T& input)
	{
		if (index.value < 0)
			throw std::out_of_range("Index can't be negative");
		if (static_cast<std::size_t>(index.value) > values.size())
			throw std::out_of_range("Index is out of range");
		values.insert(values.begin() + index.value, std::move(input));
	}

	void push_back(const T& input)
	{
		values.push_back(std::move(input));
	}

	ArrayOf concat(const ArrayOf& other) const
	{
		ArrayOf new_array = (*this);
		new_array.values.insert(new_array.values.end(), other.values.cbegin(), other.values.cend());
		return new_array;
	}

	void erase(Integer index)
	{
		values.erase(values.begin() + index.value);
	}

	std::vector<T> values;
};

using ArrayOfInteger = ArrayOf<Integer>;
using ArrayOfNumber = ArrayOf<Number>;
using ArrayOfString = ArrayOf<String>;
using ArrayOfBoolean = ArrayOf<Boolean>;

template <typename Type>
constexpr std::string_view type_to_string_view()
{
	if constexpr (std::same_as<Type, Integer>)
		return "INTEGER";
	else if constexpr (std::same_as<Type, Number>)
		return "NUMBER";
	else if constexpr (std::same_as<Type, String>)
		return "STRING";
	else if constexpr (std::same_as<Type, Boolean>)
		return "BOOLEAN";
	else if constexpr (std::same_as<Type, ArrayOfInteger>)
		return "ARRAY_OF_INTEGER";
	else if constexpr (std::same_as<Type, ArrayOfNumber>)
		return "ARRAY_OF_NUMBER";
	else if constexpr (std::same_as<Type, ArrayOfString>)
		return "ARRAY_OF_STRING";
	else if constexpr (std::same_as<Type, ArrayOfBoolean>)
		return "ARRAY_OF_BOOLEAN";
	else
		return "UNKNOWN";
}

} // YoloVM