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
        return vk::ApplicationInfo()
            .setPApplicationName(title.c_str())
            .setApplicationVersion(1)
            .setPEngineName("Terracota Rendering Engine")
            .setEngineVersion(1)
            .setApiVersion(VK_API_VERSION_1_0);
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
