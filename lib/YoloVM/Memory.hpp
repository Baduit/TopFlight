#pragma once

#include <map>
#include <string>
#include <string_view>
#include <algorithm>

#include <YoloVM/Value.hpp>
#include <YoloVM/Exception.hpp>

namespace YoloVM
{

template <typename Visitor>
concept MemoryVisitor = 
	requires(Visitor&& visitor, std::string_view name, const Value& value)
	{
		{ visitor(name, value) };
	};

class LoadingError: public Exception
{
	public:
		LoadingError(std::string name);

		// std::string copy is not noexcept so we disable copy
		LoadingError(const LoadingError&) = delete;
		LoadingError& operator=(const LoadingError&) = delete;

		// std::string move is no except so we can enable it
		// Moreover, an exception must have a copy or a move constructor
		LoadingError(LoadingError&&) noexcept = default;
		LoadingError& operator=(LoadingError&&) noexcept = default;

		virtual ~LoadingError() = default;

		virtual std::string_view get_exception_name() const noexcept;
		virtual const char* what() const noexcept;

	private:
		std::string _name;
		std::string _message;
};

class Memory
{
	public:
		using Address = std::size_t;

		Memory() = default;

		void store(std::string_view name, Value value);
		const Value& load(std::string_view name);
		void free(std::string_view name);

		template <MemoryVisitor Visitor>
		void visit(Visitor&& visitor)
		{
			for (const auto& [name, value]: _data)
				visitor(name, value);
		}

	private:
		std::map<std::string, Value, std::less<>> _data;
};

} // YoloVM