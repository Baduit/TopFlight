#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>

#include <Interpreter/Parser.hpp>

#include "LineElement.hpp"

class Parser
{
	public:
		std::vector<LineElement> parse_line(std::string_view line)
		{
			std::vector<LineElement> elements;
			if (line.empty())
			{
				return elements;
			}
			else if (line[0] == '#')
			{
				elements.push_back(LineElement{LineElement::Type::COMMENT, 0, line.size()});
				return elements;
			}
			else if (line[0] == '<')
			{
				// TODO : routine stuff
				return elements;
			}
			else
			{
				// TODO : instruction stuff
				return elements;
			}
		}

	private:
		std::optional<std::string> _actual_routine_name;
};