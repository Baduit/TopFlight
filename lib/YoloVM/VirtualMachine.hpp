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

#include <aggreget/aggreget.hpp>

#include <YoloVM/Value.hpp>
#include <YoloVM/Instruction.hpp>
#include <YoloVM/Routine.hpp>
#include <YoloVM/Memory.hpp>

namespace YoloVM
{

template <typename Visitor>
concept RoutineVisitor = 
	requires(Visitor&& visitor, const Routine& routine)
	{
		{ visitor(routine) };
	};

class VirtualMachine
{
	public:
		VirtualMachine(std::ostream& output_stream = std::cout);

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

		void get_at(std::string_view array_input, std::string_view index, std::string_view output);
		void store_at(std::string_view array_output, std::string_view index, Value value);
		void copy_at(std::string_view array_output, std::string_view index, std::string_view input);
		void size(std::string_view array_input, std::string_view output);
		void resize(std::string_view array_input, std::string_view new_size);
		void insert(std::string_view array_output, std::string_view index, std::string_view input);
		void push_back(std::string_view array_output, std::string_view input);
		void concat(std::string_view input_a, std::string_view input_b, std::string_view dest);
		void erase(std::string_view array_input, std::string_view index);

	public:
		struct UnknownRoutine: public Exception
		{
			UnknownRoutine(std::string_view routine_name):
				Exception("Routine named '" + std::string(routine_name) + "' not found.")
			{}
			virtual ~UnknownRoutine() = default;

			virtual std::string_view get_exception_name() const noexcept { return "YoloVM::VirtualMachine::UnknownRoutine"; }
		};

		struct UnknownInstruction: public Exception
		{
			UnknownInstruction(std::string_view instruction_name):
				Exception("Instruction named '" + std::string(instruction_name) + "' does not exist or is not implemented.")
			{}
			virtual ~UnknownInstruction() = default;

			virtual std::string_view get_exception_name() const noexcept { return "YoloVM::VirtualMachine::UnknownInstruction"; }
		};

		struct ExceptionInRoutine: public Exception
		{
			ExceptionInRoutine(const std::string& routine_name, std::string_view exception_name, std::string_view exception_what, const Instruction& instruction):
				Exception("In the routine '" + routine_name + "' error of type '" + std::string(exception_name) + "'during the instruction:\n\t" + instruction.to_string() + "\nwith the message:\n\t" + std::string(exception_what))
			{}
			virtual ~ExceptionInRoutine() = default;

			virtual std::string_view get_exception_name() const noexcept { return "YoloVM::VirtualMachine::ExceptionInRoutine"; }
		};


	private:
		template <typename MethodPtr, typename Arg>
		void call_helper(MethodPtr method_ptr, Arg&& arg)
		{
			std::apply(
				[&](auto&&... args)
				{
					(this->*method_ptr)(std::forward<decltype(args)>(args)...);
				}, aggreget::to_tuple(arg));
		}

		template <MemoryVisitor Visitor>
		void visit_variables(Visitor&& visitor)
		{
			_memory.visit(std::forward<Visitor>(visitor));
		}

		template <RoutineVisitor Visitor>
		void visit_routines(Visitor&& visitor)
		{
			for (const auto& routine: _routines)
				visitor(routine);
		}



	private:
		std::ostream& _output_stream;
		Memory _memory;
		std::vector<Routine> _routines;
};

} // YoloVM