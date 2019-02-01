#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>
#include<string>
#include<vector>
class Texture
{
public:
	std::string Name;
	GLfloat anisoFilterLevel;
	GLuint textureWidth;
	GLuint textureHeight;
	GLuint textureComponents;

public:

	GLuint textureID;
	GLenum Target;
	GLenum InternalFormat ;
	GLenum Format ;
	GLenum Type;
	GLenum FilterMin;
	GLenum FilterMax ;
	GLenum WrapS ;
	GLenum WrapT ;
	GLenum WrapR ;
	bool Mipmapping = true;

public:
	Texture();
	~Texture();
	void setTexture(const char* texturePath, std::string textureName, bool textureFlip);
	void setTextureHDR(const char* texturePath, std::string textureName, bool textureFlip);
	
	void setTextureCube(std::vector<const char*>& faces, bool textureFlip);
	void Bind(int start=-1);
	void Unbind();
	void useTexture();
	
};
#endif // !TEXTURE_H
