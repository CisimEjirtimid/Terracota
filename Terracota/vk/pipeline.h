#pragma once
#include "utils.h"
#include "shaders/utils.h"

namespace terracota::vk
{
    class pipeline
    {
        //vk::raii::Pipeline _pipeline;
        //vk::raii::PipelineLayout _layout;
    public:
        pipeline(vk::raii::Device& device, const vk::Extent2D& extent);
    };
}