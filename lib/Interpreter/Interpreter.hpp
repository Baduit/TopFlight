#pragma once

#include <string>
#include <iostream>
#include <span>

#include <YoloVM/Instruction.hpp>
#include <YoloVM/VirtualMachine.hpp>

#include "Parser.hpp"

namespace TopFlight
{

class Interpreter
{
	public:
		Interpreter(std::ostream& output_stream = std::cout);
		Interpreter(std::span<char*> args, std::ostream& output_stream = std::cout);
		Interpreter(const std::string& filename, std::ostream& output_stream = std::cout);
		Interpreter(const std::string& filename, std::span<char*> args, std::ostream& output_stream = std::cout);

		void process_file(const std::string& filename);
		void process_stream(std::istream& stream);

		const YoloVM::VirtualMachine& get_vm() const { return _vm; }
		const std::optional<YoloVM::Routine>& get_current_routine() const { return current_routine; }


	private:
		YoloVM::VirtualMachine _vm;
		std::optional<YoloVM::Routine> current_routine;
};

} // namespace TopFlight