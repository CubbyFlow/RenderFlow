#ifndef GLTF_SCENE_HPP
#define GLTF_SCENE_HPP

#include <tiny_gltf.h>
#include <Common/Vertex.hpp>
#include <functional>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <limits>
#include <string>
#include <unordered_map>

//! KHR extension list
//! (https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos)
#define KHR_DRACO_MESH_EXTENSION_NAME "KHR_draco_mesh_compression"
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

namespace Common
{
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

bool CheckRequiredExtensions(const tinygltf::Model& model);
};  // namespace GLTFExtension

/**
 * @brief GLTF scene file loader class
 * @details After parsing given gltf file, this class provides meshes and
 * material information about parsed model. This class largely referenced on
 * nvpro samples (https://github.com/nvpro-samples/vk_shaded_gltfscene)
 */
class GLTFScene
{
 public:
    using ImageCallback = std::function<void(const tinygltf::Image& image)>;

    /**
     * @brief Construct a new GLTFScene object
     */
    GLTFScene();

    /**
     * @brief Destroy the GLTFScene object
     */
    virtual ~GLTFScene();

    /**
     * @brief Load GLTFScene from the given scene filename and generate buffers
     * @param filename gltf scene file path
     * @param format desired vertex format for parsing scene
     * @param imageCallback callback function for conduct operation on
     * each texture image in gltf scene.
     * @return true if gltf scene loading success
     * @return false if gltf scene loading failed
     */
    bool Initialize(const std::string& filename, VertexFormat format,
                    ImageCallback imageCallback = nullptr);

    /**
     * @brief Update scene animation
     * @param animIndex index of animation want to play in the array
     * @param timeElapsed elapsed time in microseconds.
     * @return true if scene is modified by animation
     * @return false if scene is not modified by animation
     */
    bool UpdateAnimation(size_t animIndex, double timeElapsed);

 protected:
    struct GLTFMaterial
    {
        int shadingModel{
            0
        };  //! 0: metallic-roughness, 1: specular-glossiness

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

    struct GLTFNode
    {
        glm::mat4 world{ 1.0f };
        glm::mat4 local{ 1.0f };
        glm::vec3 translation{ 0.0f };
        glm::vec3 scale{ 1.0f };
        glm::quat rotation{ 0.0f, 0.0f, 0.0f, 0.0f };
        std::vector<size_t> primMeshes;
        std::vector<size_t> childNodes;
        int parentNode{ -1 };
        int nodeIndex{ 0 };
    };

    struct GLTFPrimMesh
    {
        unsigned int firstIndex{ 0 };
        unsigned int indexCount{ 0 };
        unsigned int vertexOffset{ 0 };
        unsigned int vertexCount{ 0 };
        int materialIndex{ 0 };

        glm::vec3 min{ 0.0f, 0.0f, 0.0f };
        glm::vec3 max{ 0.0f, 0.0f, 0.0f };
        std::string name;
    };

    struct GLTFCamera
    {
        glm::mat4 world{ 1.0f };
        glm::vec3 eye{ 0.0f };
        glm::vec3 center{ 0.0f };
        glm::vec3 up{ 0.0f, 1.0f, 0.0f };

        tinygltf::Camera camera;
    };

    struct GLTFLight
    {
        glm::mat4 world{ 1.0f };
        tinygltf::Light light;
    };

    struct GLTFSampler
    {
        enum class Interpolation
        {
            Linear = 0,
            Step = 1,
            Cubicspline = 2
        };
        Interpolation interpolation{ Interpolation::Linear };
        std::vector<float> inputs;
        std::vector<glm::vec4> outputs;
    };

    struct GLTFChannel
    {
        enum class Path
        {
            Translation = 0,
            Rotation = 1,
            Scale = 2,
            Weights = 3
        };
        Path path{ Path::Translation };
        int samplerIndex{ 0 };
        int nodeIndex{ 0 };
    };

    struct GLTFAnimation
    {
        std::string name;
        size_t samplerIndex{ 0 };
        size_t samplerCount{ 0 };
        size_t channelIndex{ 0 };
        size_t channelCount{ 0 };
    };

    struct SceneDimension
    {
        glm::vec3 min = { std::numeric_limits<float>::max(),
                          std::numeric_limits<float>::max(),
                          std::numeric_limits<float>::max() };
        glm::vec3 max = { std::numeric_limits<float>::min(),
                          std::numeric_limits<float>::min(),
                          std::numeric_limits<float>::min() };
        glm::vec3 size = { 0.0f, 0.0f, 0.0f };
        glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        float radius{ 0.0f };
    };

    std::vector<GLTFMaterial> _sceneMaterials;
    std::vector<GLTFNode> _sceneNodes;
    std::vector<GLTFPrimMesh> _scenePrimMeshes;
    std::vector<GLTFCamera> _sceneCameras;
    std::vector<GLTFLight> _sceneLights;
    std::vector<GLTFAnimation> _sceneAnims;
    std::vector<GLTFSampler> _sceneSamplers;
    std::vector<GLTFChannel> _sceneChannels;

    std::vector<glm::vec3> _positions;
    std::vector<glm::vec3> _normals;
    std::vector<glm::vec4> _tangents;
    std::vector<glm::vec4> _colors;
    std::vector<glm::vec2> _texCoords;
    std::vector<unsigned int> _indices;

    SceneDimension _sceneDim;

    /**
     * @brief Release scene source datum
     */
    void ReleaseSourceData();

 private:
    /**
     * @brief Load GLTF model from the given filename and pass it by reference.
     * @param model returns loaded tinygltf model to this pointer
     * @param filename gltf scene file path
     * @return true if tinygltf model loading is success
     * @return false if tinygltf model loading is failed
     */
    static bool LoadModel(tinygltf::Model* model, const std::string& filename);

    /**
     * @brief Parse attribute with desire type from the model.
     * @tparam Type attribute type to be retrieved from this function.
     * @param model initialized tinygltf model from gltf scene
     * @param primitive
     * @param attributes
     * @param name
     * @return true
     * @return false
     */
    template <typename Type>
    static bool GetAttributes(const tinygltf::Model& model,
                              const tinygltf::Primitive& primitive,
                              std::vector<Type>& attributes,
                              const std::string& name);

    /**
     * @brief Returns the SRT matrix combination of this node.
     * @param node parsed gltf scene node
     * @return glm::mat4 calculated local transform matrix
     */
    static glm::mat4 GetLocalMatrix(const GLTFNode& node);

    /**
     * @brief Import materials from the model
     * @param model parse materials from initialized gltf model
     */
    void ImportMaterials(const tinygltf::Model& model);

    /**
     * @brief Process mesh in the model
     * @param model
     * @param mesh
     * @param format
     * @param name
     */
    void ProcessMesh(const tinygltf::Model& model,
                     const tinygltf::Primitive& mesh, VertexFormat format,
                     const std::string& name);
    
    /**
     * @brief Process node in the model recursively.
     * @param model 
     * @param nodeIdx 
     * @param parentIndex 
     */
    void ProcessNode(const tinygltf::Model& model, int nodeIdx,
                     int parentIndex);
    //! Update world coordinates of the given node and child nodes
    void UpdateNode(size_t nodeIndex);
    //! Process animation in the model
    void ProcessAnimation(const tinygltf::Model& model,
                          const tinygltf::Animation& anim,
                          std::size_t channelOffset, std::size_t samplerOffset);
    
    /**
     * @brief Process animation channel and append it to _sceneChannels
     * @param channel 
     */
    void ProcessChannel(const tinygltf::AnimationChannel& channel);
    
    /**
     * @brief Process animation sampler and append it to _sceneSamplers
     * @param model 
     * @param sampler 
     */
    void ProcessSampler(const tinygltf::Model& model,
                        const tinygltf::AnimationSampler& sampler);
    
    /**
     * @brief Calculate the scene dimension from loaded nodes.
     */
    void CalculateSceneDimension();

    /**
     * @brief Compute the uninitialized cameras with parsed scene dimension.
     */
    void ComputeCamera();
    
    /**
     * @brief Returns a vector of data for a tinygltf::Value
     * @tparam Type 
     * @param value 
     * @return std::vector<Type> 
     */
    template <typename Type>
    [[nodiscard]] static std::vector<Type> GetVector(const tinygltf::Value& value);
    
    /**
     * @brief Returns a value for a tinygltf::Value
     * @tparam Type 
     * @param value 
     * @param name 
     * @param val 
     */
    template <typename Type>
    static void GetValue(const tinygltf::Value& value, const std::string& name,
                         Type& val);
    
    /**
     * @brief Returns texture ID for a tinygltf::Value
     * @param value 
     * @param name 
     * @param id 
     */
    static void GetTextureID(const tinygltf::Value& value,
                             const std::string& name, int& id);
    
    //! Temporary storages for processing nodes.
    std::unordered_map<size_t, std::vector<size_t>> _meshToPrimMap;
    std::vector<unsigned int> _u32Buffer;
    std::vector<unsigned short> _u16Buffer;
    std::vector<unsigned char> _u8Buffer;
};

}  // namespace Common

#include <Common/GLTFScene-Impl.hpp>

#endif