#include <iostream>
#include <sstream>

#include <replxx.hxx>

#include <Interpreter/Interpreter.hpp>

#include "Highlighter.hpp"
#include "Parser.hpp"

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		TopFlight::Interpreter vm_interpreter;

		replxx::Replxx rx;
		Highlighter highlighter(vm_interpreter);
		rx.set_highlighter_callback(highlighter);
		while (true)
		{
			char const * cinput = nullptr;
			do
			{
				cinput = rx.input("");
			}
			while ((cinput == nullptr) && (errno == EAGAIN));

			if (cinput == nullptr)
				return 0;
			
			std::stringstream program_input;
			program_input << cinput;
			vm_interpreter.process_stream(program_input);
		}
	}
	else
	{
		auto script_argc = static_cast<unsigned int>(argc) - 2;
		char** script_argv = &(argv[2]);
		TopFlight::Interpreter vm_interpreter(argv[1], std::span<char*>(script_argv, script_argc));
	}
}