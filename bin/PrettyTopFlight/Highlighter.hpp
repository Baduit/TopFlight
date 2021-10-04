#pragma once

#include <map>
#include <string>
#include <optional>

#include <replxx.hxx>

#include "LineElement.hpp"
#include "Parser.hpp"

class Highlighter
{
	using Color = replxx::Replxx::Color;
	public:
		Highlighter(TopFlight::Interpreter& interpreter):
			_parser(interpreter)
		{
			_colors_by_line_type[LineElement::Type::COMMENT] = Color::GREEN;
			_colors_by_line_type[LineElement::Type::ROUTINE_DELIMITER] = Color::BROWN;
			_colors_by_line_type[LineElement::Type::ROUTINE_NAME] = Color::BRIGHTMAGENTA;
			_colors_by_line_type[LineElement::Type::TYPENAME] = Color::CYAN;
			_colors_by_line_type[LineElement::Type::VARIABLE_NAME] = Color::MAGENTA;
			_colors_by_line_type[LineElement::Type::NUMERIC_VALUE] = Color::YELLOW;
			_colors_by_line_type[LineElement::Type::STRING] = Color::BLUE;
			_colors_by_line_type[LineElement::Type::ARRAY_DELIMITER] = Color::BRIGHTGREEN;
			_colors_by_line_type[LineElement::Type::INSTRUCTION_NAME] = Color::BRIGHTCYAN;
			_colors_by_line_type[LineElement::Type::INVALID_ELEMENT] = Color::RED;
		}

		Highlighter(const Highlighter&) = default;
		Highlighter& operator=(const Highlighter&) = default;

		Highlighter(Highlighter&&) = default;
		Highlighter& operator=(Highlighter&&) = default;

		void operator()(const std::string& context, replxx::Replxx::colors_t& colors)
		{
			auto elements = _parser.parse_line(context);
			for (const auto& e: elements)
			{
				for (auto i = e.begin; i < e.end; ++i)
					colors.at(i) = _colors_by_line_type[e.type];
			}
		}

	private:
		Parser _parser;
		std::map<LineElement::Type, replxx::Replxx::Color> _colors_by_line_type;
};

