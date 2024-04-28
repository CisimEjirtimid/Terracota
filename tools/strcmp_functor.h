#pragma once

#include <array>
#include <cstring>
#include <string>

// TODO: make this part of the 'tools' repo and make 'Terracota' dependent on it
namespace cisim
{
    struct strcmp_functor
    {
        inline bool operator()(const std::string_view& lhs, const std::string_view& rhs)
        {
            return lhs < rhs;
        }
    };
}
