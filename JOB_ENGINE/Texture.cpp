#include "Texture.h"

#include<stb/stb_image.h>
#include<iostream>
#include<vector>
Texture::Texture()
{
}
Texture::~Texture()
{

}
void Texture::setTexture(const char* texturePath, std::string textureName, bool textureFlip)
{
	this->Target = GL_TEXTURE_2D;
	this->Name = textureName;

	if (textureFlip)
		stbi_set_flip_vertically_on_load(true);
	else
		stbi_set_flip_vertically_on_load(false);

	glGenTextures(1, &this->textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoFilterLevel);  // Request the maximum level of anisotropy the GPU used can support and use it
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->anisoFilterLevel);


	int width, height, nrComponents;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrComponents,0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		std::cout << "-------------------------------------------" << std::endl;
		std::cout << "Texture Name :" << this->Name << std::endl;
		std::cout << "Width=" << width << std::endl;
		std::cout << "Height=" << height << std::endl;
		std::cout << "nrComponents=" << nrComponents << std::endl;
		std::cout << "-------------------------------------------" << std::endl;

		glBindTexture(GL_TEXTURE_2D, this->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << texturePath << std::endl;
		stbi_image_free(data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(int start)
{
	if (start >= 0)
		glActiveTexture(GL_TEXTURE0 + start);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}
void Texture::Unbind()
{
	glBindTexture(this->Target, 0);

}
void Texture::useTexture()
{
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void Texture::setTextureHDR(const char* texturePath, std::string textureName, bool textureFlip)
{
	std::string tempPath = std::string(texturePath);

	if (textureFlip)
		stbi_set_flip_vertically_on_load(true);
	else
		stbi_set_flip_vertically_on_load(false);

	glGenTextures(1, &this->textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoFilterLevel);  // Request the maximum level of anisotropy the GPU used can support and use it
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->anisoFilterLevel);

	if (stbi_is_hdr(tempPath.c_str()))
	{
		int width, height, nrComponents;
		unsigned char* data = stbi_load(texturePath, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
			{
				this->InternalFormat = GL_RGB32F;
				this->Format = GL_RGB;
				
			}
			else if (nrComponents == 4)
			{
				this->InternalFormat = GL_RGBA32F;
				this->Format = GL_RGBA;				
			}
			//glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << texturePath << std::endl;
			stbi_image_free(data);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setTextureCube(std::vector<const char*>& faces, bool textureFlip)
{

}