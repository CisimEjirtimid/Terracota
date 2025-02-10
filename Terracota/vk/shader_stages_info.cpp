#include "shader_stages_info.h"

namespace terracota::vk
{
    shader_stages_info::shader_stages_info(const raii::Device& device, const std::map<ShaderStageFlagBits, shaders::descriptor>& shaders)
    {
        for (auto& [stage, descriptor] : shaders)
        {
            auto module =
                raii::ShaderModule
                {
                    device,
                    ShaderModuleCreateInfo{}
                        .setCode(descriptor.code)
                };

            _info.push_back(PipelineShaderStageCreateInfo{}
                .setStage(stage)
                .setModule(module)
                .setPName(descriptor.entrypoint.c_str())
            );

            _modules.emplace(stage, std::move(module));
        }
    }

    const std::vector<PipelineShaderStageCreateInfo>& shader_stages_info::operator()() const
    {
        return _info;
    }
}