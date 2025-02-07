/*
    Terracota - Experimental Vulkan Rendering Engine
*/

#include "application.h"
#include "cen/context.h"
#include <iostream>

namespace
{
    // TODO: customization options for `application_info`
    //       possibly through some config file (toml, lua?)
    vk::ApplicationInfo application_info(const std::string& title)
    {
        // vk::ApplicationInfo allows the programmer to specifiy some basic information about the
        // program, which can be useful for layers and tools to provide more debug information.
        return vk::ApplicationInfo{
            .pApplicationName = title.c_str(),
            .applicationVersion = 1,
            .pEngineName = "Terracota Rendering Engine",
            .engineVersion = 1,
            .apiVersion = vk::ApiVersion13,
        };
    }
}

int main()
{
    try
    {
        terracota::cen::context centurion{ "Terracota", cen::iarea{ 1280, 720 } };
        terracota::vk::context vulkan{ centurion.window, terracota::vk::instance_info{
                application_info("Terracota"),
                cen::vk::required_extensions() } };

        terracota::application app{ centurion, vulkan };

        app.run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
