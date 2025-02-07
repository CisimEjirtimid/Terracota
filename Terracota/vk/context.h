#pragma once
#include <tuple>
#include <concepts>
#include "instance_info.h"
#include "device_info.h"

#include "cen/utils.h"

namespace cisim::tuple
{
    namespace concepts
    {
        template<typename T, std::size_t N>
        concept has_tuple_element =
            requires(T t)
            {
                typename std::tuple_element_t<N, std::remove_const_t<T>>;
                { get<N>(t) } -> std::convertible_to<const std::tuple_element_t<N, T>&>;
            };

        template<typename T>
        concept tuple_like = !std::is_reference_v<T> &&
            requires(T t)
            {
                typename std::tuple_size<T>::type;
                requires std::derived_from<
                    std::tuple_size<T>,
                    std::integral_constant<std::size_t, std::tuple_size_v<T>>
                >;
            } &&
            []<std::size_t... N>(std::index_sequence<N...>)
            {
                return (has_tuple_element<T, N> && ...);
            }(std::make_index_sequence<std::tuple_size_v<T>>());
    }

    // lhs subset of rhs
    template <std::size_t... Is>
    bool is_subset(concepts::tuple_like auto lhs, concepts::tuple_like auto rhs, std::index_sequence<Is...>)
    {
        return ((!std::get<Is>(lhs) || (std::get<Is>(lhs) && std::get<Is>(rhs))) && ...);
    }

    // lhs subset of rhs
    bool is_subset(concepts::tuple_like auto lhs, concepts::tuple_like auto rhs)
    {
        static_assert(std::is_same_v<decltype(lhs), decltype(rhs)>, "lhs and rhs tuples are not the same");

        return is_subset(lhs, rhs, std::make_index_sequence<std::tuple_size_v<decltype(lhs)>>{});
    }

    template<typename... Ts>
    constexpr auto of_types(concepts::tuple_like auto tuple)
    {
        auto get_element = [](auto el)
        {
            if constexpr ((std::is_same_v<decltype(el), Ts> || ...))
                return std::make_tuple(std::move(el));
            else
                return std::make_tuple();
        };

        return std::apply([&](auto ... args)
        {
            return std::tuple_cat(get_element(std::move(args)) ...);
        } , std::move(tuple));
    }
}

namespace terracota::vk
{
    namespace concepts
    {
        template <typename T>
        concept physical_device_features =
            std::same_as<T, vk::PhysicalDeviceFeatures> ||
            std::same_as<T, vk::PhysicalDeviceVulkan11Features> ||
            std::same_as<T, vk::PhysicalDeviceVulkan12Features> ||
            std::same_as<T, vk::PhysicalDeviceVulkan13Features>;
    }

    struct context
    {
        vk::raii::Context raii_context;
        vk::raii::Instance instance;
        vk::raii::SurfaceKHR surface;

        vk::raii::PhysicalDevice physical_device;

        vk::queue_infos qi;

        vk::device_info di;
        vk::raii::Device device;

        vk::raii::CommandPool command_pool;
        vk::raii::CommandBuffers command_buffers;

        context(cen::window& window, const vk::instance_info& ii);
    };
}