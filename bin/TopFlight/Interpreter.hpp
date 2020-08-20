#pragma once

#include <string>
#include <fstream>

#include <YoloVM/Instruction.hpp>
#include <YoloVM/VirtualMachine.hpp>

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
			unsigned int line_id = 0;
			while (std::getline(file, line))
			{
				++line_id;
				try
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
				catch (std::exception& e)
				{
					std::cout << "Error on line number : " << line_id << "\n" << line << std::endl;
					break;
				}
			}
		}

	private:
		VirtualMachine _vm;
};