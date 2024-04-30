#pragma once

#include <string>

class OptionsParser
{

    static constexpr int ARGUMENT_COUNT{ 6 };

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::string computeShaderPath;
    std::string initShaderPath;

    size_t flockSize;

public:

    OptionsParser(int argc, char* argv[]);

    std::string getVertexShaderPath() const;

    std::string getFragmentShaderPath() const;
    
    std::string getComputeShaderPath() const;

    std::string getInitShaderPath() const;

    size_t getFlockSize() const;
};

