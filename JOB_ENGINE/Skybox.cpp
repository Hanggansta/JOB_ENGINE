#include "Skybox.h"


Skybox::Skybox()
{

}
Skybox::~Skybox()
{

}

void Skybox::setSkyboxTexture(const char* texturePath)
{
	this->texture.setTextureHDR(texturePath, "cubemapHDR", true);
}

void Skybox::renderToShader(Shader& shader, glm::mat4& projection, glm::mat4& view)
{
	shader.Use();
	this->texture.Bind(0);
	shader.setInt("envMap", 0);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setFloat("cameraAperture", this->cameraAperture);
	shader.setFloat("cameraShutterSpeed", this->cameraShutterSpeed);
	shader.setFloat("cameraISO",this->cameraISO);

}

void Skybox::setExposure(GLfloat aperture, GLfloat shutterSpeed, GLfloat iso)
{
	this->cameraAperture = aperture;
	this->cameraShutterSpeed = shutterSpeed;
	this->cameraISO = iso;
}


