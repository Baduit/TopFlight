#pragma once

#include <variant>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <concepts>
#include <compare>
#include <optional>

#include <YoloVM/Types.hpp>
#include <YoloVM/Exception.hpp>

namespace YoloVM
{

// Note: a Value is passed by const& by default, except if it is known that the value is an Integer, Number or Boolean
class Value
{
	using Variant = std::variant<Integer, Number, Char, String, Boolean, ArrayOfInteger, ArrayOfNumber, ArrayOfString, ArrayOfBoolean>;
	public:
		template <typename T>
		explicit Value(T value):
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

		Value get_at(Value index) const;
		void store_at(Value index, Value value);
		Value size() const;
		void resize(Value new_size);
		void insert(Value index, const Value& input);
		void push_back(const Value& input);
		Value concat(const Value& other) const;
		void erase(Value index);

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

struct InvalidIndexType: public Exception
{
	InvalidIndexType(const std::string& operation);

	virtual ~InvalidIndexType() = default;

	virtual std::string_view get_exception_name() const noexcept;
};

} // YoloVM