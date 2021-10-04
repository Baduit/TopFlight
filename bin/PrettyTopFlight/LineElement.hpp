#pragma once

#include <string_view>

struct LineElement
{
	enum class Type
	{
		COMMENT,
		ROUTINE_DELIMITER,
		ROUTINE_NAME,
		TYPENAME,
		VARIABLE_NAME,
		NUMERIC_VALUE,
		STRING,
		ARRAY_DELIMITER,
		INSTRUCTION_NAME,
		INVALID_ELEMENT
	};

	Type type = Type::INVALID_ELEMENT;
	std::size_t begin = 0;
	std::size_t end = 0; // not included
};