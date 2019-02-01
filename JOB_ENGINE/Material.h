#ifndef MATERIAL_H
#define MATERIAL_H

#include<glad/glad.h>
#include<vector>
#include<tuple>
#include "Shader.h"
#include "Texture.h"
class Material
{
public:
	GLuint ID;
	Shader MaterialShader;
	std::vector<std::tuple<std::string, Texture>>textureList;


public:

	Material();
	~Material();

	void addTexture(std::string uniformName, Texture texture);
	
	void setShader(Shader& shader);
	void renderToShader();

	void clear();
	
	
	
	
	




};
#endif // !MATERIAL
