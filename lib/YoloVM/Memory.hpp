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

class Memory
{
	public:
		using Address = std::size_t;

		Memory() = default;

		void store(std::string_view name, Value value);
		const Value& load(std::string_view name) const;
		Value& load(std::string_view name);
		void free(std::string_view name);

		template <MemoryVisitor Visitor>
		void visit(Visitor&& visitor)
		{
			for (const auto& [name, value]: _data)
				visitor(name, value);
		}

		public:
			class LoadingError: public Exception
			{
				public:
					LoadingError(std::string_view name);
					virtual ~LoadingError() = default;

					virtual std::string_view get_exception_name() const noexcept;
			};

	private:
		std::map<std::string, Value, std::less<>> _data;
};

} // YoloVM