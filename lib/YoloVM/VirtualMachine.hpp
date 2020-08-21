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

#include <YoloVM/Value.hpp>
#include <YoloVM/Instruction.hpp>
#include <YoloVM/Routine.hpp>
#include <YoloVM/Memory.hpp>



enum class Operation
{
	ADD,
	SUBSTRACT,
	MUTLIPLY,
	DIVIDE,
	MODULO
	// Todo: complete with logic and binary arithm
};

class VirtualMachine
{
	public:
		VirtualMachine() = default;

		void execute_instruction(const Instruction& instruction);

		void store(std::string_view dest, Value value);
		const Value& load(std::string_view input);
		void copy(std::string_view input, std::string_view dest);
		void free(std::string_view dest);
		void print(std::string_view input);
		void add_routine(Routine routine);
		void remove_routine(std::string_view routine_name);
		void execute_routine(std::string_view routine_name);

		// They are all in this template method to be a bit more organized for now
		template <Operation operation>
		void do_arithmetic(std::string_view input_a, std::string_view input_b, std::string_view output)
		{
			if constexpr (operation == Operation::ADD)
			{
				_memory.store(output, _memory.load(input_a) + _memory.load(input_b));
			}
			else if constexpr (operation == Operation::SUBSTRACT)
			{
				_memory.store(output, _memory.load(input_a) - _memory.load(input_b));
			}
			else if constexpr (operation == Operation::MUTLIPLY)
			{
				_memory.store(output, _memory.load(input_a) * _memory.load(input_b));
			}
			else if constexpr (operation == Operation::DIVIDE)
			{
				_memory.store(output, _memory.load(input_a) / _memory.load(input_b));
			}
			else if constexpr (operation == Operation::MODULO)
			{
				_memory.store(output, _memory.load(input_a) % _memory.load(input_b));
			}
		}

	private:
		Memory _memory;
		std::vector<Routine> _routines;
};
