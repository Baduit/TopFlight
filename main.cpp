#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <cstdint>
#include <map>
#include <variant>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <concepts>

#include "Value.hpp"
#include "Instruction.hpp"

class Memory
{
	public:
		using Address = std::size_t;

		Memory() = default;

		void store(std::string_view name, Value value)
		{
			// std::map really sucks ! Use a vector of pair ?
			auto it = _data.find(name);
			if (it != _data.end())
				it->second = std::move(value);
			else
				_data.insert(std::pair(name, std::move(value)));
		}

		const Value& load(std::string_view name)
		{
			using namespace std::string_literals;
			auto it = _data.find(name);
			if (it != _data.end())
				return it->second;
			else
				throw std::out_of_range("Memory can't load at the name : " + std::string(name));
		}

		void free(std::string_view name)
		{
			auto it = _data.find(name);
			if (it != _data.end())
				_data.erase(it);
		}


	private:
		std::map<std::string, Value, std::less<>> _data;
};

enum class Operation
{
	ADD,
	SUBSTRACT,
	MUTLIPLY,
	DIVIDE,
	MODULO
	// Todo: complete with logic and binary arithm
};



class VirtualMachine
{
	public:
		VirtualMachine() = default;

		void execute_instruction(const Instruction& instruction)
		{
			instruction.visit(
				[&](const auto& i)
				{
					using T = std::decay_t<decltype(i)>;
					if constexpr (std::same_as<T, Instruction::Store>)
					{
						store(i.dest, i.value);
					}
					else if constexpr (std::same_as<T, Instruction::Copy>)
					{
						copy(i.input, i.dest);
					}
					else if constexpr (std::same_as<T, Instruction::Free>)
					{
						free(i.dest);
					}
					else if constexpr (std::same_as<T, Instruction::Add>)
					{
						do_arithmetic<Operation::ADD>(i.input_a, i.input_b, i.dest);
					}
					else if constexpr (std::same_as<T, Instruction::Substract>)
					{
						do_arithmetic<Operation::SUBSTRACT>(i.input_a, i.input_b, i.dest);
					}
					else if constexpr (std::same_as<T, Instruction::Multiply>)
					{
						do_arithmetic<Operation::MUTLIPLY>(i.input_a, i.input_b, i.dest);
					}
					else if constexpr (std::same_as<T, Instruction::Divide>)
					{
						do_arithmetic<Operation::DIVIDE>(i.input_a, i.input_b, i.dest);
					}
					else if constexpr (std::same_as<T, Instruction::Modulo>)
					{
						do_arithmetic<Operation::MODULO>(i.input_a, i.input_b, i.dest);
					}
					else if constexpr (std::same_as<T, Instruction::Print>)
					{
						print(i.input);
					}
					// No else possible
				});
		}

		void store(std::string_view dest, Value value)
		{
			_memory.store(dest, std::move(value));
		}

		// Not a real instruction, it is only in the C++ API
		const Value& load(std::string_view input)
		{
			return _memory.load(input);
		}

		void copy(std::string_view input, std::string_view dest)
		{
			_memory.store(dest, _memory.load(input));
		}

		void free(std::string_view dest)
		{
			_memory.free(dest);
		}

		// They are all in this template method to be a bit more organized for now
		template <Operation operation>
		void do_arithmetic(std::string_view input_a, std::string_view input_b, std::string_view output)
		{
			if constexpr (operation == Operation::ADD)
			{
				_memory.store(output, _memory.load(input_a) + _memory.load(input_b));
			}
			else if constexpr (operation == Operation::SUBSTRACT)
			{
				_memory.store(output, _memory.load(input_a) - _memory.load(input_b));
			}
			else if constexpr (operation == Operation::MUTLIPLY)
			{
				_memory.store(output, _memory.load(input_a) * _memory.load(input_b));
			}
			else if constexpr (operation == Operation::DIVIDE)
			{
				_memory.store(output, _memory.load(input_a) / _memory.load(input_b));
			}
			else if constexpr (operation == Operation::MODULO)
			{
				_memory.store(output, _memory.load(input_a) % _memory.load(input_b));
			}
		}

		void print(std::string_view input)
		{
			_memory.load(input).print(std::cout);
		}

	private:
		Memory _memory;
};

int main()
{
	{
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
	}
	{
		auto print_c = Instruction::from_string("PRINT c");
	}
}