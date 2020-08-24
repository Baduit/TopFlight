#pragma once

#include <string>
#include <variant>
#include <string_view>
#include <stdexcept>
#include <optional>
#include <utility>
#include <vector>
#include <cctype>
#include <charconv>
#include <sstream>

#include <external/brigand/brigand.hpp>
#include <external/aggreget/aggreget.hpp>

#include <YoloVM/BrigandExtension.hpp>

#include <YoloVM/Value.hpp>

namespace YoloVM
{

struct Instruction
{
	/*
	** Misc
	*/
	struct Store
	{
		static constexpr std::string_view NAME = "STORE";

		std::string dest;
		Value value;
	};

	struct Copy
	{
		static constexpr std::string_view NAME = "COPY";

		std::string input;
		std::string dest;
	};

	struct Free
	{
		static constexpr std::string_view NAME = "FREE";
		
		std::string dest;
	};

	struct Print
	{
		static constexpr std::string_view NAME = "PRINT";
		
		std::string input;
	};

	struct Call
	{
		static constexpr std::string_view NAME = "CALL";
		
		std::string routine_name;
	};

	struct CallIf
	{
		static constexpr std::string_view NAME = "CALL_IF";
		
		std::string routine_name;
		std::string boolean_input;
	};

	/*
	** Arithmetic
	*/
	struct Add
	{
		static constexpr std::string_view NAME = "ADD";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Substract
	{
		static constexpr std::string_view NAME = "SUBSTRACT";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Multiply
	{
		static constexpr std::string_view NAME = "MULTIPLY";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Divide
	{
		static constexpr std::string_view NAME = "DIVIDE";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct Modulo
	{
		static constexpr std::string_view NAME = "MODULO";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	/*
	** Logical
	*/
	struct LogicalAnd
	{
		static constexpr std::string_view NAME = "LOGICAL_AND";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct LogicalOr
	{
		static constexpr std::string_view NAME = "LOGICAL_OR";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct LogicalNot
	{
		static constexpr std::string_view NAME = "LOGICAL_NOT";
		
		std::string input;
		std::string dest;
	};

	/*
	** Comparaison
	*/
	struct CompareEqual
	{
		static constexpr std::string_view NAME = "COMPARE_EQUAL";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct CompareDifferent
	{
		static constexpr std::string_view NAME = "COMPARE_DIFFERENT";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct CompareLess
	{
		static constexpr std::string_view NAME = "COMPARE_LESS";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct CompareLessOrEqual
	{
		static constexpr std::string_view NAME = "COMPARE_LESS_OR_EQUAL";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct CompareGreater
	{
		static constexpr std::string_view NAME = "COMPARE_GREATER";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	struct CompareGreaterOrEqual
	{
		static constexpr std::string_view NAME = "COMPARE_GREATER_OR_EQUAL";
		
		std::string input_a;
		std::string input_b;
		std::string dest;
	};

	using InstructionTypes = brigand::list<
				Store, Copy, Free, Print, Call, CallIf,
				Add, Substract, Multiply, Divide, Modulo,
				LogicalAnd, LogicalOr, LogicalNot,
				CompareEqual, CompareDifferent, CompareLess, CompareLessOrEqual, CompareGreater, CompareGreaterOrEqual
			>;

	using Variant = brigand_list_as_variant<InstructionTypes>;

	template <typename T>
	Instruction(T i):
		_variant(std::move(i))
	{}

	Instruction() = default;

	Instruction(const Instruction&) = default;
	Instruction& operator=(const Instruction&) = default;

	Instruction(Instruction&&) = default;
	Instruction& operator=(Instruction&&) = default;

	template <typename F>
	auto visit(F&& f) const
	{
		return std::visit(std::forward<F>(f), _variant);
	}

	Variant _variant;

	inline friend std::ostream& operator<<(std::ostream& os, const Instruction& instr)
	{
		instr.visit(
			[&](const auto& i)
			{
				using InstructionType = std::decay_t<decltype(i)>;
				constexpr auto type_name = InstructionType::NAME;
				os << type_name << " => ";

				aggreget::foreach(i,
					[&](const auto& attr)
					{
						using AttrType = std::decay_t<decltype(attr)>;
						if constexpr (std::same_as<AttrType, std::string>)
						{
							os << "String: " << attr << " ";
						}
						else if constexpr (std::same_as<AttrType, YoloVM::Value>)
						{
							os << "Value: ";
							attr.print(os);
							os << " ";
						}
						else
						{
							os << "wtf";
						}
					});
			});
		return os;
	}
};


} // YoloVM