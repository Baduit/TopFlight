#pragma once

#include <string>
#include <vector>

#include <YoloVM/Instruction.hpp>

namespace YoloVM
{

struct Routine
{
	std::string name;
	std::vector<Instruction> instructions;
};

} // YoloVM