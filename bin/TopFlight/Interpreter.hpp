#pragma once

#include <string>
#include <fstream>

#include <YoloVM/Instruction.hpp>
#include <YoloVM/VirtualMachine.hpp>

#include "Parser.hpp"

namespace TopFlight
{

class Interpreter
{
	public:
		Interpreter() = default;
		Interpreter(const std::string& filename);

		void load_file(const std::string& filename);

	private:
		YololVM::VirtualMachine _vm;
};

} // namespace TopFlight