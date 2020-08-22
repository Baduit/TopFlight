#include "Parser.hpp"

namespace TopFlight
{

namespace Parser
{

namespace Impl
{

auto get_next_word(std::string_view str, char delim) -> std::pair<std::string_view, std::optional<std::string_view>>
{
	auto i = str.find(delim);
	if (i == std::string_view::npos)
		return { str, {} };
	else
		return { str.substr(0, i), str.substr(i + 1)};
}

int to_int(std::string_view str)
{
	for (auto c: str)
		if (std::isdigit(c) == 0)
			throw std::runtime_error("Invalid integer format");
	int i;
	std::stringstream ss;
	ss << str;
	ss >> i;
	if (ss.bad())
		throw std::runtime_error("Invalid integer format");
	return i;
}

double to_double(std::string_view str)
{
	for (auto c: str)
		if (std::isdigit(c) == 0 && c != '.')
			throw std::runtime_error("Invalid integer format");
	double i;
	std::stringstream ss;
	ss << str;
	ss >> i;
	if (ss.bad())
		throw std::runtime_error("Invalid integer format");
	return i;
}

std::string extract_string(std::string_view str)
{
	if (str.size() < 2 || str.front() != '"' || str.back() != '"')
		throw std::runtime_error("Invalid string format");

	std::string_view chars = str.substr(1, str.size() - 2);			
	std::string result;
	result.reserve(chars.size());
	std::size_t i = 0;
	while (i < chars.size())
	{
		char c = chars[i];
		if (c != '\\')
		{
			result += c;
		}
		else
		{
			++i;
			if (i >= chars.size())
				throw std::runtime_error("Invalid string format");
			char c2 = chars[i];
			if (c2 == '\\' || c2 == '"')
				result += c2;
			else if (c2 == '0')
				result += '\0';
			else if (c2 == 'b')
				result += '\b';
			else if (c2 == 'n')
				result += '\n';
			else if (c2 == 'r')
				result += '\r';
			else if (c2 == 't')
				result += '\t';
			else if (c2 == 'f')
				result += '\f';
			else if (c2 == 'v')
				result += '\v';
			else
				throw std::runtime_error("Invalid string format unsupported char after \\");
		}
		++i;
	}
	return result;
}

bool to_bool(std::string_view str)
{
	if (str == "true")
		return true;
	else if (str == "false")
		return false;
	else
		throw std::runtime_error("Invalid bool parameter");
}

auto parse_and_create_value(std::string_view str) -> std::pair<YoloVM::Value, std::optional<std::string_view>>
{
	auto [type, remaning_after_type] = get_next_word(str, '(');
	if (!remaning_after_type || remaning_after_type->empty())
		throw std::runtime_error("Invalid value");

	auto [values, end] = get_next_word(*remaning_after_type, ')');
	if (values.empty() || !end)
		throw std::runtime_error("Invalid value");

	std::optional<std::string_view> remaining_string;
	if (!end->empty())
	{
		remaining_string = end->substr(1);
	}

	if (type == "INTEGER")
	{
		return { YoloVM::Integer(to_int(values)), remaining_string };
	}
	else if (type == "NUMBER")
	{
		return { YoloVM::Number(to_double(values)), remaining_string };
	}
	else if (type == "STRING")
	{
		return { YoloVM::String(extract_string(values)), remaining_string };
	}
	else if (type == "BOOLEAN")
	{
		return { YoloVM::Boolean(to_bool(values)), remaining_string };
	}
	else if (type == "ARRAY_OF_INTEGER")
	{
		throw std::runtime_error("Not supported yet");
	}
	else if (type == "ARRAY_OF_NUMBER")
	{
		throw std::runtime_error("Not supported yet");
	}
	else if (type == "ARRAY_OF_STRING")
	{
		throw std::runtime_error("Not supported yet");
	}
	else if (type == "ARRAY_OF_BOOLEAN")
	{
		throw std::runtime_error("Not supported yet");
	}
	else
	{
		throw std::runtime_error("Invalid type");
	}
	
	throw std::runtime_error("Unknown value while parsing");
}

void check_routine_name(std::string_view routine_name)
{
	if (routine_name.empty())
		throw std::runtime_error("Empty routine name is not possible");

	for (char c: routine_name)
	{
		if (std::isalnum(c) == 0 && c != '_')
			throw std::runtime_error("Routine can only contain alphanumerics character or _ : " + std::string(routine_name));
	}
}


} // namespace Impl


bool is_line_a_routine_command(std::string_view line)
{
	return line.front() == '<' && line.back() == '>';
}

std::optional<RoutineCommand> get_routine_command(std::string_view line)
{
	if (line.size() < 2)
		return {};

	if (line.front() != '<' || line.back() != '>')
		return {};

	if (line[1] == '/')
	{
		RoutineCommand cmd { std::string(line.substr(2, line.size() - 3)), RoutineCommand::Type::END};
		Impl::check_routine_name(cmd.name);
		return cmd;
	}
	else
	{
		RoutineCommand cmd { std::string(line.substr(1, line.size() - 2)), RoutineCommand::Type::BEGIN};
		return cmd;
	}
}

} // namespace Parser


YoloVM::Instruction InstructionParser::parse(std::string_view str)
{
	std::optional<YoloVM::Instruction> result;
	auto [instruction, remaining_string] = Parser::Impl::get_next_word(str);

	brigand::for_each<YoloVM::Instruction::InstructionTypes>(
		[&](auto type)
		{
			using InstructionType = decltype(type)::type;
			constexpr auto type_name = InstructionType::NAME;
			if (type_name == instruction)
			{
				result = construct_instruction<0, InstructionType>(remaining_string);
			}
		});
	if (!result)
		throw std::runtime_error("Error while parsing unknwown instruction");

	// Check for no extra arguments
	return *result;
}


} // namespace TopFlight