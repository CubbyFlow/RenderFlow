#ifndef GLTF_MATERIAL_HPP
#define GLTF_MATERIAL_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace Common
{

//! KHR extension list
//! (https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos)
#define KHR_LIGHTS_PUNCTUAL_EXTENSION_NAME "KHR_lights_punctual"
#define KHR_MATERIALS_CLEARCOAT_EXTENSION_NAME "KHR_materials_clearcoat"
#define KHR_MATERIALS_PBR_SPECULAR_GLOSSINESS_EXTENSION_NAME \
    "KHR_materials_pbrSpecularGlossiness"
#define KHR_MATERIALS_SHEEN_EXTENSION_NAME "KHR_materials_sheen"
#define KHR_MATERIALS_TRANSMISSION_EXTENSION_NAME "KHR_materials_transmission"
#define KHR_MATERIALS_UNLIT_EXTENSION_NAME "KHR_materials_unlit"
#define KHR_MATERIALS_VARIANTS_EXTENSION_NAME "KHR_materials_variants"
#define KHR_MESH_QUANTIZATION_EXTENSION_NAME "KHR_mesh_quantization"
#define KHR_TEXTURE_TRANSFORM_EXTENSION_NAME "KHR_texture_transform"

namespace GLTFExtension
{
struct KHR_materials_clearcoat
{
    float factor{ 0.0f };
    int texture{ -1 };
    float roughnessFactor{ 0.0f };
    int roughnessTexture{ -1 };
    int normalTexture{ -1 };
};

struct KHR_materials_pbrSpecularGlossiness
{
    glm::vec4 diffuseFactor{ 1.0f, 1.0f, 1.0f, 1.0f };
    int diffuseTexture{ -1 };
    glm::vec3 specularFactor{ 1.0f, 1.f, 1.0f };
    float glossinessFactor{ 1.0f };
    int specularGlossinessTexture{ -1 };
};

struct KHR_materials_sheen
{
    glm::vec3 colorFactor{ 0.0f, 0.0f, 0.0f };
    int colorTexture{ -1 };
    float roughnessFactor{ 0.0f };
    int roughnessTexture{ -1 };
};

struct KHR_materials_transmission
{
    float factor{ 0.0f };
    int texture{ -1 };
};

struct KHR_materials_unlit
{
    int active{ 0 };
};

struct KHR_texture_transform
{
    glm::vec2 offset{ 0.0f, 0.0f };
    float rotation{ 0.0f };
    glm::vec2 scale{ 1.0f };
    int texCoord{ 0 };
    glm::mat3 uvTransform{
        1
    };  // Computed transform of offset, rotation, scale
};

/**
 * @brief
 *
 * @param model
 * @return true
 * @return false
 */
bool CheckRequiredExtension(const std::string& model);
};  // namespace GLTFExtension

/**
 * @brief Collection of asset loader functions.
 *
 */
struct GLTFMaterial
{
    int shadingModel{ 0 };  //! 0: metallic-roughness, 1: specular-glossiness

    //! pbrMetallicRoughness
    glm::vec4 baseColorFactor{ 1.0f, 1.0f, 1.0f, 1.0f };
    int baseColorTexture{ -1 };
    float metallicFactor{ 1.0f };
    float roughnessFactor{ 1.0f };
    int metallicRoughnessTexture{ -1 };

    int emissiveTexture{ -1 };
    glm::vec3 emissiveFactor{ 0.0f, 0.0f, 0.0f };
    int alphaMode{ 0 };  //! 0 : OPAQUE, 1 : MASK, 2 : BLEND
    float alphaCutoff{ 0.5f };
    int doubleSided{ 0 };

    int normalTexture{ -1 };
    float normalTextureScale{ 1.0f };
    int occlusionTexture{ -1 };
    float occlusionTextureStrength{ 1.0f };

    //! Extensions
    GLTFExtension::KHR_materials_pbrSpecularGlossiness specularGlossiness;
    GLTFExtension::KHR_texture_transform textureTransform;
    GLTFExtension::KHR_materials_clearcoat clearcoat;
    GLTFExtension::KHR_materials_sheen sheen;
    GLTFExtension::KHR_materials_transmission transmission;
    GLTFExtension::KHR_materials_unlit unlit;
};

};  // namespace Common

#endif  //! end of GLTFMaterial.hpp