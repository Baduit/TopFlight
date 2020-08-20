#pragma once

#include <string>
#include <vector>

#include "Instruction.hpp"

struct Routine
{
	std::string name;
	std::vector<Instruction> instructions;
};