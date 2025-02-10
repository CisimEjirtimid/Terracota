/*
    Terracota - Experimental Vulkan Rendering Engine
*/

#include "application.h"
#include "cen/context.h"
#include <iostream>

namespace
{
    vk::ApplicationInfo application_info()
    {
        // TODO: customization options for `application_info`
        //       possibly through some config file (toml, lua?)
        //       * use ConfigBuilder or something similar
        static constexpr auto application_name{ "Terracota" };
        static constexpr auto engine_name{ "Terracota Rendering Engine" };

        // vk::ApplicationInfo allows the programmer to specifiy some basic information about the
        // program, which can be useful for layers and tools to provide more debug information.
        return vk::ApplicationInfo{
            .pApplicationName = application_name,
            .applicationVersion = 1,
            .pEngineName = engine_name,
            .engineVersion = 1,
            .apiVersion = vk::ApiVersion13,
        };
    }
}

int main()
{
    try
    {
        auto app_info = application_info();

        terracota::cen::context centurion{ app_info.pApplicationName, cen::iarea{ 1280, 720 } };

        terracota::vk::context vulkan{ centurion.window,
            terracota::vk::instance_info{
                app_info,
                cen::vk::required_extensions()
            },
            terracota::vk::physical_device_requirements{
                .queues = {
                    terracota::vk::queue_family::graphics,
                    terracota::vk::queue_family::presentation
                },
                .device_type = terracota::vk::PhysicalDeviceType::eDiscreteGpu,
                .api_version = app_info.apiVersion,
                .features10 = {
                    .geometryShader = true,
                    .tessellationShader = true,
                    .shaderFloat64 = true,
                },
                .features12 = {
                    .descriptorIndexing = true,
                    .bufferDeviceAddress = true
                },
                .features13 = {
                    .synchronization2 = true,
                    .dynamicRendering = true
                }
            }
        };

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
