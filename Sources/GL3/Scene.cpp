#include <GL3/Scene.hpp>
#include <GL3/Shader.hpp>
#include <GL3/Texture.hpp>
#include <bitset>
#include <glad/glad.h>

namespace GL3 {

	struct GltfShadeMaterial
	{
		int shadingModel;  // 0: metallic-roughness, 1: specular-glossiness

		// PbrMetallicRoughness
		glm::vec4  pbrBaseColorFactor;
		int   pbrBaseColorTexture;
		float pbrMetallicFactor;
		float pbrRoughnessFactor;
		int   pbrMetallicRoughnessTexture;

		// KHR_materials_pbrSpecularGlossiness
		glm::vec4  khrDiffuseFactor;
		int   khrDiffuseTexture;
		glm::vec3  khrSpecularFactor;
		float khrGlossinessFactor;
		int   khrSpecularGlossinessTexture;

		int   emissiveTexture;
		glm::vec3  emissiveFactor;
		int   alphaMode;
		float alphaCutoff;
		int   doubleSided;

		int   normalTexture;
		float normalTextureScale;
		int   occlusionTexture;
		float occlusionTextureStrength;
	};

	Scene::Scene()
	{
		//! Do nothing
	}

	Scene::~Scene()
	{
		//! Do nothing
	}

	bool Scene::Initialize(const std::string& filename, Core::VertexFormat format)
	{
		std::cout << "Loading Scene : " << filename << '\n';
		if (!Core::GLTFScene::Initialize(filename, format, [&](const tinygltf::Image& image) {
			std::string name = image.name.empty() ? std::string("texture") + std::to_string(this->_textures.size()) : image.name;
			std::cout << "Loading Image : " << name << '\n';
			auto texture = std::make_shared<GL3::Texture>();
			texture->Initialize(GL_TEXTURE_2D);
			texture->UploadTexture(&image.image[0], image.width, image.height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
			_textures.emplace_back(std::move(texture));
		}))
			return false;
		
		//! vertex buffer storages index
		int index = 0;

		//! Resize vertex buffer storage with number of vertex attribute type in given format,
		//! additionally index buffer also added.
		_buffers.resize(std::bitset<static_cast<unsigned int>(Core::VertexFormat::Last)>(
						static_cast<unsigned int>(format)).count() + 1);

		//! Create & Bind vertex array object
		glCreateVertexArrays(1, &_vao);
		glCreateBuffers(_buffers.size(), _buffers.data());

		//! Temporary buffer binding lambda function
		auto bindingBuffer = [&](void* data, size_t num, Core::VertexFormat attribute) {
			if (static_cast<int>(format & attribute))
			{
				const size_t numFloats = Core::VertexHelper::GetNumberOfFloats(attribute);
				const size_t stride = numFloats * sizeof(float);
				glNamedBufferStorage(_buffers[index], num * stride, data, GL_DYNAMIC_STORAGE_BIT);
				glVertexArrayVertexBuffer(_vao, index, _buffers[index], 0, stride);
				glEnableVertexArrayAttrib(_vao, index);
				glVertexArrayAttribFormat(_vao, index, numFloats, GL_FLOAT, GL_FALSE, 0);
				glVertexArrayAttribBinding(_vao, index, index);
				++index;
			}
		};

		//! Create & Bind the vertex buffers
		bindingBuffer(_positions.data(), _positions.size(), Core::VertexFormat::Position3);
		bindingBuffer(_normals.data(),	 _normals.size(),	Core::VertexFormat::Normal3	 );
		bindingBuffer(_tangents.data(),  _tangents.size(),	Core::VertexFormat::Tangent4 );
		bindingBuffer(_colors.data(),	 _colors.size(),	Core::VertexFormat::Color4	 );
		bindingBuffer(_texCoords.data(), _texCoords.size(), Core::VertexFormat::TexCoord2);

		//! Create buffers for indices
		glCreateBuffers(1, &_ebo);
		glNamedBufferStorage(_ebo, _indices.size() * sizeof(unsigned int), _indices.data(), GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(_vao, _ebo);

		//! Create shader storage buffer object for matrices per-instance.
		std::vector<NodeMatrix> matrices;
		for (const auto& node : _sceneNodes)
		{
			NodeMatrix instance;
			instance.first = node.world;
			instance.second = glm::transpose(node.world);
			matrices.emplace_back(std::move(instance));
		}
		
		glGenBuffers(1, &_matrixBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _matrixBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _matrixBuffer);

		return true;
	}

	void Scene::Render(const std::shared_ptr< Shader >& shader, GLenum alphaMode)
	{
		(void)shader; (void)alphaMode;
		glBindVertexArray(_vao);

		int lastMaterialIdx = -1, nodeIdx = 0;

		for (int i = 0; i < static_cast<int>(_textures.size()); ++i)
			_textures[i]->BindTexture(i);

		for (auto& node : _sceneNodes)
		{
			const size_t uboOffset = sizeof(NodeMatrix) * nodeIdx;
			(void)uboOffset;

			auto& primMesh = _scenePrimMeshes[node.primMesh];
			if (primMesh.materialIndex != lastMaterialIdx)
			{
				GLTFMaterial& mat = _sceneMaterials[primMesh.materialIndex];
				shader->SendUniformVariable("shadingModel", mat.shadingModel);
				shader->SendUniformVariable("pbrBaseColorFactor", mat.baseColorFactor);
				shader->SendUniformVariable("pbrBaseColorTexture", mat.baseColorTexture);
				shader->SendUniformVariable("pbrMetallicFactor", mat.metallicFactor);
				shader->SendUniformVariable("pbrRoughnessFactor", mat.roughnessFactor);
				shader->SendUniformVariable("pbrMetallicRoughnessTexture", mat.metallicRoughnessTexture);
				shader->SendUniformVariable("khrDiffuseFactor", mat.specularGlossiness.diffuseFactor);
				shader->SendUniformVariable("khrDiffuseTexture", mat.specularGlossiness.diffuseTexture);
				shader->SendUniformVariable("khrSpecularFactor", mat.specularGlossiness.specularFactor);
				shader->SendUniformVariable("khrGlossinessFactor", mat.specularGlossiness.glossinessFactor);
				shader->SendUniformVariable("khrSpecularGlossinessTexture", mat.specularGlossiness.specularGlossinessTexture);
				shader->SendUniformVariable("emissiveTexture", mat.emissiveTexture);
				shader->SendUniformVariable("emissiveFactor", mat.emissiveFactor);
				shader->SendUniformVariable("alphaMode", mat.alphaMode);
				shader->SendUniformVariable("alphaCutoff", mat.alphaCutoff);
				shader->SendUniformVariable("doubleSided", mat.doubleSided);
				shader->SendUniformVariable("normalTexture", mat.normalTexture);
				shader->SendUniformVariable("normalTextureScale", mat.normalTextureScale);
				shader->SendUniformVariable("occlusionTexture", mat.occlusionTexture);
				shader->SendUniformVariable("occlusionTextureStrength", mat.occlusionTextureStrength);
				lastMaterialIdx = primMesh.materialIndex;
			}
			
			shader->SendUniformVariable("instanceIdx", nodeIdx);

			//! Draw elements with primitive mesh index informations.
			glDrawElementsBaseVertex(GL_TRIANGLES, primMesh.indexCount, GL_UNSIGNED_INT, 
				reinterpret_cast<const void*>(primMesh.firstIndex * sizeof(unsigned int)), primMesh.vertexOffset);

			++nodeIdx;
		}

		glBindVertexArray(0);
	}

	void Scene::CleanUp()
	{
		_textures.clear();
		glDeleteBuffers(1, &_matrixBuffer);
		glDeleteBuffers(_buffers.size(), _buffers.data());
		glDeleteBuffers(1, &_ebo);
		glDeleteVertexArrays(1, &_vao);
	}

};