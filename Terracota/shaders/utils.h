#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace shaders
{
    std::vector<uint32_t> read(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Failed to open a shader file!");

        size_t size = (size_t)file.tellg();
        std::vector<uint32_t> buffer(size / 4);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), size);

        return buffer;
    }
}