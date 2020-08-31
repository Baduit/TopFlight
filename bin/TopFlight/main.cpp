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
		TopFlight::Interpreter vm_interpreter(argv[1]);
	}
}