#include <typeinfo>

#include <YoloVM/Value.hpp>

namespace YoloVM
{

Value operator+(const Value& a, const Value& b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;
					if constexpr (!Addable<A, B>)
					{
						throw ImpossibleOperation("add", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value + b_value);
					}
				}, b._variant);

		}, a._variant);
}

Value operator-(const Value& a, const Value& b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;
					if constexpr (!Substracable<A, B>)
					{
						throw ImpossibleOperation("substract", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value - b_value);
					}
				}, b._variant);

		}, a._variant);
}

Value operator*(const Value& a, const Value& b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;
					if constexpr (!Multiplyable<A, B>)
					{
						throw ImpossibleOperation("multiply", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value * b_value);
					}
				}, b._variant);

		}, a._variant);
}

Value operator/(const Value& a, const Value& b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;
					if constexpr (!Dividable<A, B>)
					{
						throw ImpossibleOperation("divide", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value / b_value);
					}
				}, b._variant);

		}, a._variant);
}

Value operator%(const Value& a, const Value& b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;
					if constexpr (!Modulable<A, B>)
					{
						throw ImpossibleOperation("modulo", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value % b_value);
					}
				}, b._variant);

		}, a._variant);
}

Value operator&&(Value a, Value b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;
					if constexpr (!HasLogicAnd<A, B>)
					{
						throw ImpossibleOperation("logic and", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value && b_value);
					}
				}, b._variant);

		}, a._variant);
}

Value operator||(Value a, Value b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;
					if constexpr (!HasLogicOr<A, B>)
					{
						throw ImpossibleOperation("logic or", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value || b_value);
					}
				}, b._variant);

		}, a._variant);
}

Value Value::operator!() const
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;

			if constexpr (!HasLogicNot<A>)
			{
				throw ImpossibleOperation("logic not", type_to_string_view<A>());
				return Value(); // Ugly I know
			}
			else
			{
				return Value(!a_value);
			}

		}, _variant);
}

bool operator==(const Value& a, const Value& b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;

					if constexpr (!std::same_as<A, B>)
					{
						throw ImpossibleOperation("check equality", type_to_string_view<A>(), type_to_string_view<B>());
						return false; // Ugly I know
					}
					else
					{
						return a_value == b_value;
					}
				}, b._variant);

		}, a._variant);
}

std::strong_ordering operator<=>(const Value& a, const Value& b)
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;

					if constexpr (!std::same_as<A, B>)
					{
						throw ImpossibleOperation("compare", type_to_string_view<A>(), type_to_string_view<B>());
						return std::strong_ordering::equivalent; // Ugly I know
					}
					else
					{
						return a_value <=> b_value;
					}
				}, b._variant);

		}, a._variant);
}

void Value::print(std::ostream& out) const
{
	return std::visit(
		[&](const auto& value)
		{
			value.print(out);
		}, _variant);
}

ImpossibleOperation::ImpossibleOperation(const std::string& operation, std::string_view first_type)
{
	_message = "Impossible to do the operation : \"" + operation + "\" on a variable of type '" + std::string(first_type) + "'";
}

ImpossibleOperation::ImpossibleOperation(const std::string& operation, std::string_view first_type, std::string_view second_type)
{
	_message = "Impossible to do the operation : \"" + operation + "\" on a variables of type '" + std::string(first_type) + "' and '" + std::string(second_type) + "'";
}

std::string_view ImpossibleOperation::get_exception_name() const noexcept
{
	return "YoloVM::ImpossibleOperation";
}

const char* ImpossibleOperation::what() const noexcept
{
	return _message.c_str();
}

} // YoloVM