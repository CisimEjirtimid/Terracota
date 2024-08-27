#pragma once
#include "context.h"
#include "swap_chain_info.h"

namespace terracota::vk
{
    class swap_chain
    {
        vk::swap_chain_info _info;
        vk::raii::SwapchainKHR _swap_chain;
        std::vector<vk::raii::ImageView> _framebuffer_views;

    public:
        swap_chain(context& context, const Extent2D& framebuffer_size);

        std::vector<vk::raii::ImageView>& framebuffer_views();
    };
}