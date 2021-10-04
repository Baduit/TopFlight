#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <stdexcept>

#include <Interpreter/Parser.hpp>
#include <Interpreter/Interpreter.hpp>

#include "LineElement.hpp"

class Parser
{
	public:
		Parser(TopFlight::Interpreter& interpreter):
			_interpreter(interpreter)
		{}

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
						elements.push_back(LineElement{LineElement::Type::ROUTINE_DELIMITER, i, i + 1});
						is_line_complete = true;
						
						if (line[1] != '/')
						{
							if (!_interpreter.get_current_routine())
								elements.push_back(LineElement{LineElement::Type::ROUTINE_NAME, begin, i});
							else
								elements.push_back(LineElement{LineElement::Type::INVALID_ELEMENT, begin, i});
						}
						else
						{
							std::string_view routine_name = line.substr(begin, i - begin);
							if (_interpreter.get_current_routine() && routine_name == _interpreter.get_current_routine()->name)
								elements.push_back(LineElement{LineElement::Type::ROUTINE_NAME, begin, i});
							else
							{
								elements.push_back(LineElement{LineElement::Type::INVALID_ELEMENT, begin, i});
							}
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
				auto [instruction, remaining_string] = TopFlight::Parser::Impl::get_next_word(line);
				if (remaining_string)
				{
					bool is_instruction_correct = false;
					brigand::for_each<YoloVM::Instruction::InstructionTypes>(
						[&](auto type)
						{
							using InstructionType = typename decltype(type)::type;
							constexpr auto type_name = InstructionType::NAME;
							if (type_name == instruction)
							{
								elements.push_back(LineElement{LineElement::Type::INSTRUCTION_NAME, 0, instruction.size()});
								is_instruction_correct = true;
							}
						});
					if (!is_instruction_correct)
					{
						elements.push_back(LineElement{LineElement::Type::INVALID_ELEMENT, 0, line.size()});
						return elements;
					}
				}
				return elements;
			}
		}

	private:
		TopFlight::Interpreter& _interpreter;
};