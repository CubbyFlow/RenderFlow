#include <Common/GLTFMaterial.hpp>
#include <unordered_set>

namespace Common
{
bool GLTFExtension::CheckRequiredExtension(const std::string& extension)
{
    static std::unordered_set<std::string> kSupportedExtensions{
        KHR_LIGHTS_PUNCTUAL_EXTENSION_NAME,
        KHR_MATERIALS_CLEARCOAT_EXTENSION_NAME,
        KHR_MATERIALS_PBR_SPECULAR_GLOSSINESS_EXTENSION_NAME,
        KHR_MATERIALS_SHEEN_EXTENSION_NAME,
        KHR_MATERIALS_TRANSMISSION_EXTENSION_NAME,
        KHR_MATERIALS_UNLIT_EXTENSION_NAME,
        KHR_MATERIALS_VARIANTS_EXTENSION_NAME,
        KHR_MESH_QUANTIZATION_EXTENSION_NAME,
        KHR_TEXTURE_TRANSFORM_EXTENSION_NAME,
    };

    return kSupportedExtensions.find(extension) != kSupportedExtensions.end();
}

}  // namespace Common