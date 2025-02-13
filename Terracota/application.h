#pragma once

#include "cen/context.h"
#include "vk/context.h"
#include "vk/queues.h"
#include "vk/swap_chain.h"
#include "vk/frame_data.h"
//#include "vk/pipeline.h"

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

            std::array<vk::frame_data, vk::FRAME_OVERLAP> frames;
            uint64_t frames_drawn = 0;
            //vk::pipeline pipeline;

            v(const cen::window& window, vk::context& context);

            size_t _current_frame_idx = 0;
            vk::frame_data& current_frame();
            void swap();
        } _vulkan;

        void loop();

    public:
        application(cen::context& centurion, vk::context& vk);
        ~application();

        void run();
    };
}

