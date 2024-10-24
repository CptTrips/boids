#pragma once

#include <string>

class OptionsParser
{

    static constexpr int ARGUMENT_COUNT{ 3 };

    std::string shaderFolder;

    uint32_t flockSize;

public:

    OptionsParser(int argc, char* argv[]);

    std::string getShaderFolder() const;

    uint32_t getFlockSize() const;
};

