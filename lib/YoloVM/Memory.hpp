#pragma once

#include <map>
#include <string>
#include <string_view>

#include <YoloVM/Value.hpp>

namespace YololVM
{

class Memory
{
	public:
		using Address = std::size_t;

		Memory() = default;

		void store(std::string_view name, Value value);
		const Value& load(std::string_view name);
		void free(std::string_view name);


	private:
		std::map<std::string, Value, std::less<>> _data;
};

} // YololVM