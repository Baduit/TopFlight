#include <fstream>

#include "Interpreter.hpp"

namespace TopFlight
{

Interpreter::Interpreter(std::ostream& output_stream):
	_vm(output_stream)
{}

Interpreter::Interpreter(std::span<char*> args, std::ostream& output_stream):
	_vm(output_stream)
{
	std::vector<YoloVM::String> owned_args;
	owned_args.reserve(args.size());
	for (const char* a: args)
		owned_args.emplace_back(a);
	_vm.store("args", YoloVM::Value(YoloVM::ArrayOfString(std::move(owned_args))));
}

Interpreter::Interpreter(const std::string& filename, std::ostream& output_stream):
	_vm(output_stream)
{
	process_file(filename);
}

Interpreter::Interpreter(const std::string& filename, std::span<char*> args, std::ostream& output_stream):
	_vm(output_stream)
{
	std::vector<YoloVM::String> owned_args;
	owned_args.reserve(args.size());
	for (const char* a: args)
		owned_args.emplace_back(a);
	_vm.store("args", YoloVM::Value(YoloVM::ArrayOfString(std::move(owned_args))));
	process_file(filename);
}

void Interpreter::process_file(const std::string& filename)
{
	std::ifstream file(filename);
	process_stream(file);
}

void Interpreter::process_stream(std::istream& stream)
{
	std::string line;
	unsigned int line_id = 0;
	while (std::getline(stream, line))
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
					current_routine = YoloVM::Routine{ routine_cmd->name, {} };
				}
				else
				{
					if (!current_routine)
						throw std::runtime_error("Impossible to end a routine which noes not exist");
					if (current_routine->name != routine_cmd->name)
						throw std::runtime_error("Wrong closing routine name");
					_vm.add_routine(std::move(*current_routine));
					current_routine.reset();
				}
			}
			else
			{
				if (current_routine)
					current_routine->instructions.push_back(InstructionParser::parse(line));
				else
					_vm.execute_instruction(InstructionParser::parse(line));
			}
		}
		catch(YoloVM::Exception& e)
		{
			std::cout
					<< "Error on line number : " << line_id << "\n"
					<< "Line: " << line << "\n"
					<< "Error: " << e.get_exception_name() << "\n"
					<< "Message: " << e.what() << std::endl;
			break;
		}
		catch (std::exception& e)
		{
			std::cout
					<< "Error on line number : " << line_id << "\n"
					<< "Line: " << line << "\n"
					<< "Error: " << e.what() << std::endl;
			break;
		}
	}
}

} // namespace TopFlight