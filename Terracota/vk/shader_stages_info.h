#pragma once
#include <map>
#include "utils.h"
#include "shaders/utils.h"

namespace terracota::vk
{
    class shader_stages_info
    {
        std::map<ShaderStageFlagBits, raii::ShaderModule> _modules;
        std::vector<PipelineShaderStageCreateInfo> _info;
    public:
        shader_stages_info(const raii::Device& device, const std::map<ShaderStageFlagBits, shaders::descriptor>& shaders);

        const std::vector<PipelineShaderStageCreateInfo>& operator()() const;
    };
}