#pragma once

#include <string>
#include <fstream>

#include "Instruction.hpp"
#include "VirtualMachine.hpp"
#include "Parser.hpp"

class Interpreter
{
	public:
		Interpreter() = default;
		Interpreter(const std::string& filename)
		{
			load_file(filename);
		}

		void load_file(const std::string& filename)
		{
			std::ifstream file(filename);
			std::string line;
			while (std::getline(file, line))
			{
				if (line.empty() || line.front() == '#')
					continue;

				if (line.back() == '\r')
					line.pop_back();

				// In case it becomes empty after the removal of the '\r' if there is one
				if (line.empty())
					continue;

				_vm.execute_instruction(Parser::parse_instruction(line));
			}
		}

	private:
		VirtualMachine _vm;
};