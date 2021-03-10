#include <GL3/Texture.hpp>
#include <GL3/DebugUtils.hpp>
#include <glad/glad.h>
#include <iostream>

namespace GL3 {

	Texture::Texture()
		: _target(GL_TEXTURE_2D), _textureID(0)
	{
		//! Do nothing
	}

	Texture::~Texture()
	{
		CleanUp();
	}

	void Texture::Initialize(GLenum target)
	{
		_target = target;

		glCreateTextures(target, 1, &_textureID);
		glTextureParameteri(_textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(_textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture::UploadTexture(void* data, int width, int height, GLenum format, GLenum internalFormat, GLenum type)
	{
		glTextureStorage2D(_textureID, 1, internalFormat, width, height);
		glTextureSubImage2D(_textureID, 0, 0, 0, width, height, format, type, data);
		glGenerateTextureMipmap(_textureID);
	}

	void Texture::BindTexture(GLuint slot) const
	{
		glBindTextureUnit(slot, _textureID);
	}

	void Texture::UnbindTexture() const
	{
		glBindTexture(_target, 0);
	}

	void Texture::CleanUp()
	{
		if (_textureID) glDeleteTextures(1, &_textureID);
	}

};