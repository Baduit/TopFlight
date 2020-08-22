#include <YoloVM/VirtualMachine.hpp>

namespace YoloVM
{

void VirtualMachine::execute_instruction(const Instruction& instruction)
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
				add(i.input_a, i.input_b, i.dest);
			}
			else if constexpr (std::same_as<T, Instruction::Substract>)
			{
				substract(i.input_a, i.input_b, i.dest);
			}
			else if constexpr (std::same_as<T, Instruction::Multiply>)
			{
				multiply(i.input_a, i.input_b, i.dest);
			}
			else if constexpr (std::same_as<T, Instruction::Divide>)
			{
				divide(i.input_a, i.input_b, i.dest);
			}
			else if constexpr (std::same_as<T, Instruction::Modulo>)
			{
				modulo(i.input_a, i.input_b, i.dest);
			}
			else if constexpr (std::same_as<T, Instruction::Print>)
			{
				print(i.input);
			}
			else if constexpr (std::same_as<T, Instruction::Call>)
			{
				execute_routine(i.routine_name);
			}
			else if constexpr (std::same_as<T, Instruction::LogicalAnd>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::LogicalOr>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::LogicalNot>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::CompareEqual>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::CompareDifferent>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::CompareLess>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::CompareLessOrEqual>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::CompareGreater>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else if constexpr (std::same_as<T, Instruction::CompareGreaterOrEqual>)
			{
				throw std::runtime_error("Instruction not implemented.");
			}
			else
			{
				throw std::runtime_error("Instruction unknown or not implemented.");
			}
			
		});
}

void VirtualMachine::store(std::string_view dest, Value value)
{
	_memory.store(dest, std::move(value));
}

// Not a real instruction, it is only in the C++ API
const Value& VirtualMachine::load(std::string_view input)
{
	return _memory.load(input);
}

void VirtualMachine::copy(std::string_view input, std::string_view dest)
{
	_memory.store(dest, _memory.load(input));
}

void VirtualMachine::free(std::string_view dest)
{
	_memory.free(dest);
}


void VirtualMachine::print(std::string_view input)
{
	_memory.load(input).print(std::cout);
}

void VirtualMachine::add_routine(Routine routine)
{
	auto it = std::find_if(_routines.begin(), _routines.end(),
		[&](const auto& r)
		{
			return r.name == routine.name;
		});
	if (it != _routines.end())
		it->instructions = std::move(routine.instructions);
	else
		_routines.push_back(std::move(routine));
}

void VirtualMachine::remove_routine(std::string_view routine_name)
{
	auto it = std::find_if(_routines.begin(), _routines.end(),
		[&](const auto& r)
		{
			return r.name == routine_name;
		});
	if (it != _routines.end())
		_routines.erase(it);
}

void VirtualMachine::execute_routine(std::string_view routine_name)
{
	auto it = std::find_if(_routines.begin(), _routines.end(),
		[&](const auto& r)
		{
			return r.name == routine_name;
		});
	if (it == _routines.end())
		throw std::runtime_error("Routine not found");
	
	for (const auto& instruction: it->instructions)
		execute_instruction(instruction);
}

void VirtualMachine::add(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, _memory.load(input_a) + _memory.load(input_b));
}

void VirtualMachine::substract(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, _memory.load(input_a) - _memory.load(input_b));
}

void VirtualMachine::multiply(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, _memory.load(input_a) * _memory.load(input_b));
}

void VirtualMachine::divide(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, _memory.load(input_a) / _memory.load(input_b));
}

void VirtualMachine::modulo(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, _memory.load(input_a) % _memory.load(input_b));
}


} // YoloVM