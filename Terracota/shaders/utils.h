#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace shaders
{
    struct descriptor
    {
        std::vector<uint32_t> code;
        std::string entrypoint{ "main" };
    };

    inline descriptor read(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Failed to open a shader file!");

        size_t size = (size_t)file.tellg();
        std::vector<uint32_t> buffer(size / 4);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), size);

        return { buffer }; // TODO: entrypoint can be something other than default `main`
    }
}