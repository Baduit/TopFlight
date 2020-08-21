#pragma once

#include <variant>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <concepts>

#include <YoloVM/Types.hpp>
#include <YoloVM/OperatorConcepts.hpp>

namespace YololVM
{

enum class Type
{
	// Value are used as index in a variant
	INTEGER = 0,
	NUMBER = 1,
	BYTE = 2,
	ARRAY_OF_INTEGER = 3,
	ARRAY_OF_NUMBER = 4,
	ARRAY_OF_BYTE = 5
};

class Value
{
	using Variant = std::variant<Integer, Number, String, Boolean, ArrayOfInteger, ArrayOfNumber, ArrayOfString, ArrayOfBoolean>;
	public:
		template <typename T>
		Value(T value):
			_variant(std::move(value))
		{}

		Value() = default;

		Value(const Value&) = default;
		Value& operator=(const Value&) = default;

		Value(Value&&) = default;
		Value& operator=(Value&&) = default;

		friend Value operator+(const Value& a, const Value& b);
		friend Value operator-(const Value& a, const Value& b);
		friend Value operator*(const Value& a, const Value& b);
		friend Value operator/(const Value& a, const Value& b);
		friend Value operator%(const Value& a, const Value& b);

		void print(std::ostream& out) const;

	private:
		Variant _variant;
};

} // YololVM