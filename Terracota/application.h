#pragma once

#include "cen/context.h"
#include "vk/context.h"
#include "vk/queues.h"
#include "vk/swap_chain.h"

namespace terracota
{
    class application
    {
        cen::context& _centurion;

        struct v
        {
            vk::context& context;

            vk::queues queues;
            vk::swap_chain swap_chain;

            v(const cen::window& window, vk::context& context);
        } _vulkan;

        void loop();

    public:
        application(cen::context& centurion, vk::context& vk);
        ~application();

        void run();
    };
}

