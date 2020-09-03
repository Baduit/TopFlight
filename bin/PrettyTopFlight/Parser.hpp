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
				elements.push_back(LineElement{LineElement::Type::ROUTINE_DELIMITER, 0, 1});
				std::size_t begin = 1;
				if (line.size() > 1 && line[1] == '/')
				{
					elements.push_back(LineElement{LineElement::Type::ROUTINE_DELIMITER, 1, 2});
					++begin;
				}

				bool is_line_complete = false;
				auto i = begin;
				for (; i < line.size(); ++i)
				{
					if (line[i] == '>')
					{
						is_line_complete = true;
						if (line[1] != '/')
						{
							elements.push_back(LineElement{LineElement::Type::ROUTINE_DELIMITER, i, i + 1});
							elements.push_back(LineElement{LineElement::Type::ROUTINE_NAME, begin, i});
							_last_beginned_routine_name = std::string(line.substr(i, i + 1));
						}
						else
						{
							std::string_view routine_name = line.substr(i, i + 1);
							if (routine_name == _last_beginned_routine_name)
								elements.push_back(LineElement{LineElement::Type::ROUTINE_NAME, begin, i});
							else
								elements.push_back(LineElement{LineElement::Type::INVALID_ELEMENT, begin, i});
						}
						break;
					}
				}

				if (!is_line_complete)
					elements.push_back(LineElement{LineElement::Type::ROUTINE_NAME, begin, i});
				return elements;
			}
			else
			{
				// TODO : instruction stuff
				return elements;
			}
		}

	private:
		std::optional<std::string> _last_beginned_routine_name;
};