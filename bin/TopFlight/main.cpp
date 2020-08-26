#include <iostream>

#include <YoloVM/Instruction.hpp>
#include <YoloVM/VirtualMachine.hpp>
#include <Interpreter/Interpreter.hpp>

int main(int argc, char** argv)
{
	if (argc <= 1)
		throw std::invalid_argument("Missing filename");

	TopFlight::Interpreter vm_interpreter(argv[1]);
}