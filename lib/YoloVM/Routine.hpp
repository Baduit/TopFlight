#pragma once

#include <string>
#include <vector>

#include <YoloVM/Instruction.hpp>

struct Routine
{
	std::string name;
	std::vector<Instruction> instructions;
};