#include "shader_stages_info.h"

namespace terracota::vk
{
    shader_stages_info::shader_stages_info(const vk::raii::Device& device, const std::map<vk::ShaderStageFlagBits, shaders::descriptor>& shaders)
    {
        for (auto& [stage, descriptor] : shaders)
        {
            auto module =
                vk::raii::ShaderModule
                {
                    device,
                    vk::ShaderModuleCreateInfo{}
                        .setCode(descriptor.code)
                };

            _info.push_back(vk::PipelineShaderStageCreateInfo{}
                .setStage(stage)
                .setModule(module)
                .setPName(descriptor.entrypoint.c_str())
            );

            _modules.emplace(stage, std::move(module));
        }
    }

    const std::vector<vk::PipelineShaderStageCreateInfo>& shader_stages_info::operator()() const
    {
        return _info;
    }
}