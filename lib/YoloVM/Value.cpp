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

					if constexpr (!std::same_as<A, B>)
					{
						throw std::invalid_argument("Type mismatch");
						return Value(); // Ugly I know
					}
					else
					{
						if constexpr (!HasLogicAnd<A>)
						{
							throw std::invalid_argument("Operation impossible for this type");
							return Value(); // Ugly I know
						}
						else
						{
							return Value(a_value && b_value);
						}
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

					if constexpr (!std::same_as<A, B>)
					{
						throw std::invalid_argument("Type mismatch");
						return Value(); // Ugly I know
					}
					else
					{
						if constexpr (!HasLogicOr<A>)
						{
							throw std::invalid_argument("Operation impossible for this type");
							return Value(); // Ugly I know
						}
						else
						{
							return Value(a_value || b_value);
						}
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

			if constexpr (!HasLogicAnd<A>)
			{
				throw std::invalid_argument("Operation impossible for this type");
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
						throw std::invalid_argument("Type mismatch");
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
						throw std::invalid_argument("Type mismatch");
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

} // YoloVM