#ifndef SKYBOX_H
#define SKYBOX_H

#include<glad/glad.h>
#include "Shader.h"
#include "Texture.h"
class Skybox
{
public:
	GLfloat cameraAperture;//取景框
	GLfloat cameraShutterSpeed;//快门速度
	GLfloat cameraISO; //感光度
	Texture texture;
	
	Skybox();
	
	~Skybox();
	

	void setSkyboxTexture(const char* texturePath);

	void renderToShader(Shader& skyboxShader, glm::mat4& projection, glm::mat4& view);

	void setExposure(GLfloat aperture, GLfloat shutterSpeed, GLfloat iso);
	//设置曝光度
	












};
#endif // !SKYBOX_H
