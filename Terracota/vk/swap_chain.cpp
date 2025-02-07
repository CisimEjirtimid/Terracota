#include "swap_chain.h"

namespace terracota::vk
{
    swap_chain::swap_chain(context& context, const Extent2D& framebuffer_size)
        : _info{
            context.physical_device, context.surface,
            vk::swap_chain_info::params{
                .framebuffer_size = framebuffer_size,
                .queue_family_indices = context.qi.family_indices()
            }
        }
        , _swap_chain{ context.device, _info() }
    {
        // template for all framebuffer image views
        auto framebuffer_info = vk::ImageViewCreateInfo{}
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(_info().imageFormat)
            .setComponents(vk::ComponentMapping{}
                .setR(vk::ComponentSwizzle::eIdentity)
                .setG(vk::ComponentSwizzle::eIdentity)
                .setB(vk::ComponentSwizzle::eIdentity)
                .setA(vk::ComponentSwizzle::eIdentity))
            .setSubresourceRange(vk::ImageSubresourceRange{}
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1));

        for (auto& framebuffer : _swap_chain.getImages())
            _framebuffer_views.push_back(vk::raii::ImageView{ context.device, framebuffer_info.setImage(framebuffer) });
    }

    const vk::swap_chain_info& swap_chain::info() const
    {
        return _info;
    }

    std::vector<vk::raii::ImageView>& swap_chain::framebuffer_views()
    {
        return _framebuffer_views;
    }
}