#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <cstdint>
#include <variant>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <concepts>
#include <algorithm>

#include <external/aggreget/aggreget.hpp>

#include <YoloVM/Value.hpp>
#include <YoloVM/Instruction.hpp>
#include <YoloVM/Routine.hpp>
#include <YoloVM/Memory.hpp>

namespace YoloVM
{

enum class ArithmeticOperation
{
	ADD,
	SUBSTRACT,
	MUTLIPLY,
	DIVIDE,
	MODULO
};

class VirtualMachine
{
	public:
		VirtualMachine() = default;

		void execute_instruction(Instruction instruction);

		void store(std::string_view dest, Value value);
		const Value& load(std::string_view input);
		void copy(std::string_view input, std::string_view dest);
		void free(std::string_view dest);
		void print(std::string_view input);

		void add_routine(Routine routine);
		void remove_routine(std::string_view routine_name);
		void execute_routine(std::string_view routine_name);
		void execute_routine_if(std::string_view routine_name, std::string_view boolean_input);

		void add(std::string_view input_a, std::string_view input_b, std::string_view output);
		void substract(std::string_view input_a, std::string_view input_b, std::string_view output);
		void multiply(std::string_view input_a, std::string_view input_b, std::string_view output);
		void divide(std::string_view input_a, std::string_view input_b, std::string_view output);
		void modulo(std::string_view input_a, std::string_view input_b, std::string_view output);

		void logical_and(std::string_view input_a, std::string_view input_b, std::string_view output);
		void logical_or(std::string_view input_a, std::string_view input_b, std::string_view output);
		void logical_not(std::string_view input, std::string_view output);

		void compare_equal(std::string_view input_a, std::string_view input_b, std::string_view output);
		void compare_different(std::string_view input_a, std::string_view input_b, std::string_view output);
		void compare_less(std::string_view input_a, std::string_view input_b, std::string_view output);
		void compare_less_or_equal(std::string_view input_a, std::string_view input_b, std::string_view output);
		void compare_greater(std::string_view input_a, std::string_view input_b, std::string_view output);
		void compare_greater_or_equal(std::string_view input_a, std::string_view input_b, std::string_view output);

		template <typename MethodPtr, typename Arg>
		void call_helper(MethodPtr method_ptr, Arg&& arg)
		{
			std::apply(
				[&](auto&&... args)
				{
					(this->*method_ptr)(std::forward<decltype(args)>(args)...);
				}, aggreget::to_tuple(arg));
		}

	private:
		Memory _memory;
		std::vector<Routine> _routines;
};

} // YoloVM