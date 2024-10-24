#include "OptionsParser.h"

#include <stdexcept>
#include <vector>

OptionsParser::OptionsParser(int argc, char* argv[])
{

	if (argc != ARGUMENT_COUNT)
        throw std::runtime_error("Invalid number of options");

	const std::vector<std::string> optionTokens(argv + 1, argv + argc);

	shaderFolder = optionTokens[0];

	int flockInt{ std::stoi(optionTokens[1]) };

	if (flockInt < 0)
		flockInt = 0;

	flockSize = static_cast<uint32_t>(flockInt);
}

std::string OptionsParser::getShaderFolder() const
{
	return shaderFolder;
}

uint32_t OptionsParser::getFlockSize() const
{
	return flockSize;
}

