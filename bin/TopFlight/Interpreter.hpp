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
			std::optional<Routine> current_routine;
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

					auto routine_cmd = Parser::get_routine_command(line);
					if (routine_cmd)
					{
						if (routine_cmd->type == Parser::RoutineCommand::Type::BEGIN)
						{
							if (current_routine)
								throw std::runtime_error("It is not possible to have a routine inside a routine");
							current_routine = Routine{ routine_cmd->name, {} };
						}
						else
						{
							if (!current_routine)
								throw std::runtime_error("Impossible to end a routine which noes not exist");
							_vm.add_routine(std::move(*current_routine));
							current_routine.reset();
						}
					}
					else
					{
						if (current_routine)
							current_routine->instructions.push_back(Parser::parse_instruction(line));
						else
							_vm.execute_instruction(Parser::parse_instruction(line));
					}
				}
				catch (std::exception& e)
				{
					std::cout << "Error on line number : " << line_id << "\nLine: " << line << "\nError: " << e.what() << std::endl;
					break;
				}
			}
		}

	private:
		VirtualMachine _vm;
};