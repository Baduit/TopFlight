#include <iostream>

#include <YoloVM/Instruction.hpp>
#include <YoloVM/VirtualMachine.hpp>
#include "Parser.hpp"
#include "Interpreter.hpp"

void old_tests()
{
		{
		std::cout << "VM with direct c++ calls" << std::endl;
		VirtualMachine vm;
		vm.store("a", Integer(5));
		vm.store("b", Integer(3));
		vm.do_arithmetic<Operation::ADD>("a", "b", "c");
		vm.print("c");
		vm.free("a");
		vm.free("b");
		vm.free("c");

		vm.store("str", String("\nYolol\n"));
		vm.print("str");
		vm.free("str");

		vm.store("numbers", ArrayOfNumber{ { 5.6, 5, 7.3 } });
		vm.print("numbers");
		vm.free("numbers");
		std::cout << "\n******** END ******" << std::endl;
	}
	{
		std::cout << "VM with string input as instructions" << std::endl;
		auto store_a = Parser::parse_instruction("STORE a NUMBER(7)");
		auto store_b = Parser::parse_instruction("STORE b NUMBER(8)");
		auto add_a_b = Parser::parse_instruction("ADD a b c");
		auto print_c = Parser::parse_instruction("PRINT c");
		auto copy_c_to_copied = Parser::parse_instruction("COPY c copied");
		auto print_copied = Parser::parse_instruction("PRINT copied");
		auto free_a = Parser::parse_instruction("FREE a");
		auto free_b = Parser::parse_instruction("FREE b");
		auto free_c = Parser::parse_instruction("FREE c");
		auto free_copied = Parser::parse_instruction("FREE copied");

		VirtualMachine vm;
		vm.execute_instruction(store_a);
		vm.execute_instruction(store_b);
		vm.execute_instruction(add_a_b);
		vm.execute_instruction(print_c);
		vm.execute_instruction(Parser::parse_instruction("STORE str STRING(\"\nthis is a string\n\")"));
		vm.execute_instruction(Parser::parse_instruction("PRINT str"));
		vm.execute_instruction(copy_c_to_copied);
		vm.execute_instruction(print_copied);
		vm.execute_instruction(free_a);
		vm.execute_instruction(free_b);
		vm.execute_instruction(free_c);
		vm.execute_instruction(free_copied);
		std::cout << "\n******** END ******" << std::endl;
	}
}

int main(int argc, char** argv)
{
	if (argc <= 1)
		throw std::invalid_argument("Missing filename");

	Interpreter vm_interpreter(argv[1]);
}