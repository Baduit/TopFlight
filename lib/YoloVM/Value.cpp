#include <typeinfo>

#include <YoloVM/Value.hpp>
#include <YoloVM/MethodConcepts.hpp>
#include <YoloVM/OperatorConcepts.hpp>

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

Value Value::get_at(Value index) const
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;

			if constexpr (!HasGetAt<A>)
			{
				throw ImpossibleOperation("get_at", type_to_string_view<A>());
				return Value(); // Ugly I know
			}
			else
			{
				if (auto* ptr = std::get_if<Integer>(&(index._variant)); ptr)
				{
					return Value(a_value.get_at(*ptr));
				}
				else
				{
					throw InvalidIndexType("get_at");
				}
			}

		}, _variant);
}

void Value::store_at(Value index, Value value)
{
	std::visit(
		[&](auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;

					if constexpr (!HasStoreAt<A, B>)
					{
						throw ImpossibleOperation("store_at", type_to_string_view<A>(), type_to_string_view<B>());
					}
					else
					{
						if (auto* ptr = std::get_if<Integer>(&(index._variant)); ptr)
						{
							a_value.store_at(*ptr, b_value);
						}
						else
						{
							throw InvalidIndexType("store_at");
						}
					}
				}, value._variant);

		}, _variant);
}

Value Value::size() const
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;

			if constexpr (!HasSize<A>)
			{
				throw ImpossibleOperation("size", type_to_string_view<A>());
				return Value(); // Ugly I know
			}
			else
			{
				return Value(a_value.size());
			}

		}, _variant);
}

void Value::resize(Value new_size)
{
	std::visit(
		[&](auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;

			if constexpr (!HasResize<A>)
			{
				throw ImpossibleOperation("resize", type_to_string_view<A>());
			}
			else
			{
				if (auto* ptr = std::get_if<Integer>(&(new_size._variant)); ptr)
				{
					a_value.resize(*ptr);
				}
				else
				{
					throw InvalidIndexType("resize");
				}
			}

		}, _variant);
}

void Value::insert(Value index, const Value& input)
{
	std::visit(
		[&](auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;

					if constexpr (!HasInsert<A, B>)
					{
						throw ImpossibleOperation("insert", type_to_string_view<A>(), type_to_string_view<B>());
					}
					else
					{
						if (auto* ptr = std::get_if<Integer>(&(index._variant)); ptr)
						{
							a_value.insert(*ptr, b_value);
						}
						else
						{
							throw InvalidIndexType("insert");
						}
					}
				}, input._variant);

		}, _variant);
}

void Value::push_back(const Value& input)
{
	std::visit(
		[&](auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;

					if constexpr (!HasPushBack<A, B>)
					{
						throw ImpossibleOperation("push_back", type_to_string_view<A>(), type_to_string_view<B>());
					}
					else
					{
						a_value.push_back(b_value);
					}
				}, input._variant);

		}, _variant);
}

Value Value::concat(const Value& other) const
{
	return std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;
			return std::visit(
				[&](const auto& b_value)
				{
					using B = std::decay_t<decltype(b_value)>;

					if constexpr (!HasConcat<A, B>)
					{
						throw ImpossibleOperation("concat", type_to_string_view<A>(), type_to_string_view<B>());
						return Value(); // Ugly I know
					}
					else
					{
						return Value(a_value.concat(b_value));
					}
				}, other._variant);

		}, _variant);	
}

void Value::erase(Value index)
{
	std::visit(
		[&](const auto& a_value)
		{
			using A = std::decay_t<decltype(a_value)>;

			if constexpr (!HasErase<A>)
			{
				throw ImpossibleOperation("erase", type_to_string_view<A>());
			}
			else
			{
				if (auto* ptr = std::get_if<Integer>(&(index._variant)); ptr)
				{
					Value(a_value.erase(*ptr));
				}
				else
				{
					throw InvalidIndexType("erase");
				}
			}

		}, _variant);
}

ImpossibleOperation::ImpossibleOperation(const std::string& operation, std::string_view first_type):
	Exception("Impossible to do the operation : \"" + operation + "\" on a variable of type '" + std::string(first_type) + "'")
{
}

ImpossibleOperation::ImpossibleOperation(const std::string& operation, std::string_view first_type, std::string_view second_type):
	Exception("Impossible to do the operation : \"" + operation + "\" on a variables of type '" + std::string(first_type) + "' and '" + std::string(second_type) + "'")
{
}

std::string_view ImpossibleOperation::get_exception_name() const noexcept
{
	return "YoloVM::ImpossibleOperation";
}

InvalidIndexType::InvalidIndexType(const std::string& operation):
	Exception("Impossible to do the operation : \"" + operation + "\" because the index is not an INTEGER")
{
}

std::string_view InvalidIndexType::get_exception_name() const noexcept
{
	return "YoloVM::InvalidIndexType";
}

} // YoloVM