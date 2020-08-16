#pragma once

#include <variant>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <concepts>

#include "Types.hpp"
#include "OperatorConcepts.hpp"

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
	using Variant = std::variant<Integer, Number, String, ArrayOfInteger, ArrayOfNumber, ArrayOfString>;
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

		friend Value operator+(const Value& a, const Value& b)
		{
			return std::visit(
				[&](const auto& a_value)
				{
					return std::visit(
						[&](const auto& b_value)
						{
							using A = std::decay_t<decltype(a_value)>;
							using B = std::decay_t<decltype(b_value)>;

							if constexpr (!std::same_as<A, B>)
							{
								throw std::invalid_argument("Type mismatch");
								return Value(); // Ugly I know
							}
							else
							{
								if constexpr (!Addable<A>)
								{
									throw std::invalid_argument("Operation impossible for this type");
									return Value(); // Ugly I know
								}
								else
								{
									return Value(a_value + b_value);
								}
							}
						}, b._variant);

				}, a._variant);
		}

		friend Value operator-(const Value& a, const Value& b)
		{
			return std::visit(
				[&](const auto& a_value)
				{
					return std::visit(
						[&](const auto& b_value)
						{
							using A = std::decay_t<decltype(a_value)>;
							using B = std::decay_t<decltype(b_value)>;

							if constexpr (!std::same_as<A, B>)
							{
								throw std::invalid_argument("Type mismatch");
								return Value(); // Ugly I know
							}
							else
							{
								if constexpr (!Substracable<A>)
								{
									throw std::invalid_argument("Operation impossible for this type");
									return Value(); // Ugly I know
								}
								else
								{
									return Value(a_value - b_value);
								}
							}
						}, b._variant);

				}, a._variant);
		}

		friend Value operator*(const Value& a, const Value& b)
		{
			return std::visit(
				[&](const auto& a_value)
				{
					return std::visit(
						[&](const auto& b_value)
						{
							using A = std::decay_t<decltype(a_value)>;
							using B = std::decay_t<decltype(b_value)>;

							if constexpr (!std::same_as<A, B>)
							{
								throw std::invalid_argument("Type mismatch");
								return Value(); // Ugly I know
							}
							else
							{
								if constexpr (!Multiplyable<A>)
								{
									throw std::invalid_argument("Operation impossible for this type");
									return Value(); // Ugly I know
								}
								else
								{
									return Value(a_value * b_value);
								}
							}
						}, b._variant);

				}, a._variant);
		}

		friend Value operator/(const Value& a, const Value& b)
		{
			return std::visit(
				[&](const auto& a_value)
				{
					return std::visit(
						[&](const auto& b_value)
						{
							using A = std::decay_t<decltype(a_value)>;
							using B = std::decay_t<decltype(b_value)>;

							if constexpr (!std::same_as<A, B>)
							{
								throw std::invalid_argument("Type mismatch");
								return Value(); // Ugly I know
							}
							else
							{
								if constexpr (!Dividable<A>)
								{
									throw std::invalid_argument("Operation impossible for this type");
									return Value(); // Ugly I know
								}
								else
								{
									return Value(a_value / b_value);
								}
							}
						}, b._variant);

				}, a._variant);
		}

		friend Value operator%(const Value& a, const Value& b)
		{
			return std::visit(
				[&](const auto& a_value)
				{
					return std::visit(
						[&](const auto& b_value)
						{
							using A = std::decay_t<decltype(a_value)>;
							using B = std::decay_t<decltype(b_value)>;

							if constexpr (!std::same_as<A, B>)
							{
								throw std::invalid_argument("Type mismatch");
								return Value(); // Ugly I know
							}
							else
							{
								if constexpr (!Modulable<A>)
								{
									throw std::invalid_argument("Operation impossible for this type");
									return Value(); // Ugly I know
								}
								else
								{
									return Value(a_value % b_value);
								}
							}
						}, b._variant);

				}, a._variant);
		}

		void print(std::ostream& out) const
		{
			return std::visit(
				[&](const auto& value)
				{
					value.print(out);
				}, _variant);
		}

	private:
		Variant _variant;
};