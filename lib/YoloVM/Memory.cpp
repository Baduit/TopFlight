#include <YoloVM/Memory.hpp>

namespace YoloVM
{

Memory::LoadingError::LoadingError(std::string_view name):
	Exception("Impossible to load the Value named :\"" + std::string(name) + "\" in the memory.")
{}

std::string_view Memory::LoadingError::get_exception_name() const noexcept
{
	return "YoloVM::Memory::LoadingError";
}

void Memory::store(std::string_view name, Value value)
{
	// std::map really sucks ! Use a vector of pair ?
	auto it = _data.find(name);
	if (it != _data.end())
		it->second = std::move(value);
	else
		_data.insert(std::pair(name, std::move(value)));
}

const Value& Memory::load(std::string_view name) const
{
	using namespace std::string_literals;
	auto it = _data.find(name);
	if (it != _data.end())
		return it->second;
	else
		throw LoadingError(std::string(name));
}

Value& Memory::load(std::string_view name)
{
	using namespace std::string_literals;
	auto it = _data.find(name);
	if (it != _data.end())
		return it->second;
	else
		throw LoadingError(std::string(name));
}

void Memory::free(std::string_view name)
{
	auto it = _data.find(name);
	if (it != _data.end())
		_data.erase(it);
}

} // YoloVM