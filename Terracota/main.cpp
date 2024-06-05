/*
    Terracota - Experimental Vulkan Rendering Engine
*/

#include "application.h"
#include <iostream>

int main()
{
    try
    {
        terracota::application app;

        app.run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
