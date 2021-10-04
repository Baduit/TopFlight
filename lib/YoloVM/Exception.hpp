#pragma once

#include <stdexcept>
#include <string_view>

namespace YoloVM
{

class Exception: public std::runtime_error
{
	public:
		Exception(std::string_view message):
			std::runtime_error(std::string(message))
		{}

		virtual ~Exception() = default;

		// For conveniance when catching then printing
		virtual std::string_view get_exception_name() const noexcept
		{
			return "YoloVM::Exception";
		}
};

}