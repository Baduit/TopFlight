#include <iostream>
#include <sstream>

#include <replxx.hxx>

#include <Interpreter/Interpreter.hpp>

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		TopFlight::Interpreter vm_interpreter;

		replxx::Replxx rx;
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
		// In this case this is the exact same as using the ugly version
		TopFlight::Interpreter vm_interpreter(argv[1]);
	}
}