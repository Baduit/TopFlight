#pragma once

#include <variant>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <concepts>
#include <compare>
#include <optional>

#include <YoloVM/Types.hpp>
#include <YoloVM/OperatorConcepts.hpp>
#include <YoloVM/Exception.hpp>

namespace YoloVM
{

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


		friend Value operator&&(Value a, Value b);
		friend Value operator||(Value a, Value b);
		Value operator!() const;

		friend bool operator==(const Value& a, const Value& b);
		friend std::strong_ordering operator<=>(const Value& a, const Value& b);

		void print(std::ostream& out) const;

	private:
		Variant _variant;
};

class ImpossibleOperation: public Exception
{
	public:
		ImpossibleOperation(const std::string& operation, std::string_view first_type);
		ImpossibleOperation(const std::string& operation, std::string_view first_type, std::string_view second_type);

		virtual ~ImpossibleOperation() = default;

		virtual std::string_view get_exception_name() const noexcept;

};

} // YoloVM