#include <iostream>

#include <Interpreter/Interpreter.hpp>

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		TopFlight::Interpreter vm_interpreter;
		vm_interpreter.process_stream(std::cin);
	}
	else
	{
		auto script_argc = static_cast<unsigned int>(argc) - 2;
		char** script_argv = &(argv[2]);
		TopFlight::Interpreter vm_interpreter(argv[1], std::span<char*>(script_argv, script_argc));
	}
}