#include <stdexcept>
#include <iostream>

#include "OptionsParser.h"
#include "Application.h"

int main(int argc, char* argv[])
{

    try
    {

        OptionsParser options(argc, argv);

        Application app(options);

        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

