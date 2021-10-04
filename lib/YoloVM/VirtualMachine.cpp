#include <YoloVM/VirtualMachine.hpp>

namespace YoloVM
{

VirtualMachine::VirtualMachine(std::ostream& output_stream):
	_output_stream(output_stream)
{

}

void VirtualMachine::execute_instruction(Instruction instruction)
{
	//std::cout << instruction << std::endl;

	instruction.visit(
		[&](const auto& i)
		{
			using T = std::decay_t<decltype(i)>;
			if constexpr (std::same_as<T, Instruction::Store>)
			{
				call_helper(&VirtualMachine::store, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Copy>)
			{
				call_helper(&VirtualMachine::copy, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Free>)
			{
				call_helper(&VirtualMachine::free, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Add>)
			{
				call_helper(&VirtualMachine::add, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Substract>)
			{
				call_helper(&VirtualMachine::substract, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Multiply>)
			{
				call_helper(&VirtualMachine::multiply, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Divide>)
			{
				call_helper(&VirtualMachine::divide, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Modulo>)
			{
				call_helper(&VirtualMachine::modulo, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Print>)
			{
				call_helper(&VirtualMachine::print, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Call>)
			{
				call_helper(&VirtualMachine::execute_routine, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CallIf>)
			{
				call_helper(&VirtualMachine::execute_routine_if, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::LogicalAnd>)
			{
				call_helper(&VirtualMachine::logical_and, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::LogicalOr>)
			{
				call_helper(&VirtualMachine::logical_or, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::LogicalNot>)
			{
				call_helper(&VirtualMachine::logical_not, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CompareEqual>)
			{
				call_helper(&VirtualMachine::compare_equal, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CompareDifferent>)
			{
				call_helper(&VirtualMachine::compare_different, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CompareLess>)
			{
				call_helper(&VirtualMachine::compare_less, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CompareLessOrEqual>)
			{
				call_helper(&VirtualMachine::compare_less_or_equal, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CompareGreater>)
			{
				call_helper(&VirtualMachine::compare_greater, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CompareGreaterOrEqual>)
			{
				call_helper(&VirtualMachine::compare_greater_or_equal, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::GetAt>)
			{
				call_helper(&VirtualMachine::get_at, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::StoreAt>)
			{
				call_helper(&VirtualMachine::store_at, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::CopyAt>)
			{
				call_helper(&VirtualMachine::copy_at, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Size>)
			{
				call_helper(&VirtualMachine::size, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Resize>)
			{
				call_helper(&VirtualMachine::resize, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Insert>)
			{
				call_helper(&VirtualMachine::insert, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::PushBack>)
			{
				call_helper(&VirtualMachine::push_back, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Concat>)
			{
				call_helper(&VirtualMachine::concat, std::move(i));
			}
			else if constexpr (std::same_as<T, Instruction::Erase>)
			{
				call_helper(&VirtualMachine::erase, std::move(i));
			}
			else
			{
				throw UnknownInstruction(T::NAME);
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
	_memory.load(input).print(_output_stream);
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
		throw UnknownRoutine(routine_name);
	
	for (const auto& instruction: it->instructions)
	{
		try
		{
			execute_instruction(instruction);
		}
		catch (Exception& e)
		{
			throw ExceptionInRoutine(it->name, e.get_exception_name(), e.what(), instruction);
		}
		catch (std::exception& e)
		{
			throw ExceptionInRoutine(it->name, "std exception", e.what(), instruction);
		}
	}
}

void VirtualMachine::execute_routine_if(std::string_view routine_name, std::string_view boolean_input)
{
	if (load(boolean_input) != Value(Boolean(true)))
		return;

	execute_routine(routine_name);
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

void VirtualMachine::logical_and(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, _memory.load(input_a) && _memory.load(input_b));
}

void VirtualMachine::logical_or(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, _memory.load(input_a) || _memory.load(input_b));
}

void VirtualMachine::logical_not(std::string_view input, std::string_view output)
{
	_memory.store(output, !(_memory.load(input)));
}

void VirtualMachine::compare_equal(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, Value(Boolean(_memory.load(input_a) == _memory.load(input_b))));
}

void VirtualMachine::compare_different(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, Value(Boolean(_memory.load(input_a) != _memory.load(input_b))));
}

void VirtualMachine::compare_less(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, Value(Boolean(_memory.load(input_a) < _memory.load(input_b))));
}

void VirtualMachine::compare_less_or_equal(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, Value(Boolean(_memory.load(input_a) <= _memory.load(input_b))));
}

void VirtualMachine::compare_greater(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, Value(Boolean(_memory.load(input_a) > _memory.load(input_b))));
}

void VirtualMachine::compare_greater_or_equal(std::string_view input_a, std::string_view input_b, std::string_view output)
{
	_memory.store(output, Value(Boolean(_memory.load(input_a) >= _memory.load(input_b))));
}

void VirtualMachine::get_at(std::string_view array_input, std::string_view index, std::string_view output)
{
	_memory.store(output, _memory.load(array_input).get_at(_memory.load(index)));
}

void VirtualMachine::store_at(std::string_view array_output, std::string_view index, Value value)
{
	_memory.load(array_output).store_at(_memory.load(index), value);
}

void VirtualMachine::copy_at(std::string_view array_output, std::string_view index, std::string_view input)
{
	_memory.load(array_output).store_at(_memory.load(index), _memory.load(input));
}

void VirtualMachine::size(std::string_view array_input, std::string_view output)
{
	_memory.store(output, _memory.load(array_input).size());
}

void VirtualMachine::resize(std::string_view array_input, std::string_view new_size)
{
	_memory.load(array_input).resize(_memory.load(new_size));
}

void VirtualMachine::insert(std::string_view array_output, std::string_view index, std::string_view input)
{
	_memory.load(array_output).insert(_memory.load(index), _memory.load(input));
}

void VirtualMachine::push_back(std::string_view array_output, std::string_view input)
{
	_memory.load(array_output).push_back(_memory.load(input));
}

void VirtualMachine::concat(std::string_view input_a, std::string_view input_b, std::string_view dest)
{
	_memory.store(dest, _memory.load(input_a).concat(_memory.load(input_b)));
}

void VirtualMachine::erase(std::string_view array_input, std::string_view index)
{
	_memory.load(array_input).erase(_memory.load(index));
}

} // YoloVM