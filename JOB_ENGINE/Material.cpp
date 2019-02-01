#include "Material.h"`

Material::Material()
{

}

Material::~Material()
{

}

void Material::addTexture(std::string uniformName, Texture texture)
{
	this->textureList.push_back(std::tuple<std::string, Texture>(uniformName, texture));
}


void Material::setShader(Shader& shader)
{
	this->MaterialShader = shader;
	
}

void Material::renderToShader()
{
	this->MaterialShader.Use();
	
	if (textureList.size())
	{
		for (GLuint i = 0; i < this->textureList.size(); i++)
		{
			std::string currentUniformName = std::get<0>(this->textureList[i]);
			
			Texture currentTexture = std::get<1>(this->textureList[i]);

			currentTexture.Bind(i);
			
			this->MaterialShader.setInt(currentUniformName.c_str(),i);
		}
	}

}

void Material::clear()
{
	this->textureList.clear();
}