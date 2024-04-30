#include "OptionsParser.h"

#include <stdexcept>
#include <vector>

OptionsParser::OptionsParser(int argc, char* argv[])
{

	if (argc != ARGUMENT_COUNT)
        throw std::runtime_error("Invalid number of options");

	const std::vector<std::string> optionTokens(argv + 1, argv + argc);

	vertexShaderPath = optionTokens[0];
	
	fragmentShaderPath = optionTokens[1];

	computeShaderPath = optionTokens[2];

	initShaderPath = optionTokens[3];

	int flockInt{ std::stoi(optionTokens[4]) };

	if (flockInt < 0)
		flockInt = 0;

	flockSize = static_cast<size_t>(flockInt);
}

std::string OptionsParser::getVertexShaderPath() const
{

	return vertexShaderPath;
}

std::string OptionsParser::getFragmentShaderPath() const
{

	return fragmentShaderPath;
}

std::string OptionsParser::getComputeShaderPath() const
{
	return computeShaderPath;
}

std::string OptionsParser::getInitShaderPath() const
{
	return initShaderPath;
}

size_t OptionsParser::getFlockSize() const
{
	return flockSize;
}

