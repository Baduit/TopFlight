#pragma once

#include <stdexcept>
#include <string_view>

namespace YoloVM
{

class Exception: public std::exception
{
	public:
		Exception() noexcept = default;

		Exception(const Exception&) noexcept = default;
		Exception& operator=(const Exception&) noexcept = default;

		Exception(Exception&&) noexcept = default;
		Exception& operator=(Exception&&) noexcept = default;

		virtual ~Exception() = default;

		// For conveniance when catching then printing
		virtual std::string_view get_exception_name() const noexcept
		{
			return "YoloVM::Exception";
		}

		virtual const char* what() const noexcept
		{
			return get_exception_name().data();
		}
};

}