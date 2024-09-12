#pragma once
#include <map>
#include "utils.h"
#include "shaders/utils.h"

namespace terracota::vk
{
    class shader_stages_info
    {
        std::map<vk::ShaderStageFlagBits, vk::raii::ShaderModule> _modules;
        std::vector<vk::PipelineShaderStageCreateInfo> _info;
    public:
        shader_stages_info(const vk::raii::Device& device, const std::map<vk::ShaderStageFlagBits, shaders::descriptor>& shaders);

        const std::vector<vk::PipelineShaderStageCreateInfo>& operator()() const;
    };
}