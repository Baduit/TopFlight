#pragma once

#include <string>
#include <vector>

#include <YoloVM/Instruction.hpp>

namespace YololVM
{

struct Routine
{
	std::string name;
	std::vector<Instruction> instructions;
};

} // YololVM