#include "utils.h"
#include "strcmp_functor.h"

namespace terracota::vk
{
    name_vector::name_vector(std::vector<const char*>& other)
    {
        clear();
        reserve(other.size());

        for (auto c_str : other)
            emplace_back(std::string_view{ c_str });
    }

    std::vector<const char*>& name_vector::native()
    {
        _raw.clear(); // recreate `_raw` vector in case `this` vector was changed

        for (auto& name : *this)
            _raw.push_back(name.data());

        return _raw;
    }

    vk::name_vector name_vector::intersect(vk::name_vector& other)
    {
        vk::name_vector result;

        std::sort(begin(), end(), cisim::strcmp_functor{});

        std::sort(other.begin(), other.end(), cisim::strcmp_functor{});

        std::set_intersection(
            begin(), end(),
            other.begin(), other.end(),
            std::back_inserter(result), cisim::strcmp_functor{});

        return std::move(result);
    }

    name_vector extensions()
    {
        name_vector result;

        for (auto& property : enumerateInstanceExtensionProperties())
            result.push_back(property.extensionName);

        return result;
    }

    name_vector layers()
    {
        name_vector result;

        for (auto& property : enumerateInstanceLayerProperties())
            result.push_back(property.layerName);

        return result;
    }
}
