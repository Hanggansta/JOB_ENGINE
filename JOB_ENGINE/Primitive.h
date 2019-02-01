#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include<glad/glad.h>
#include<string>
#include "Shader.h"
#include "camera.h"


class Primitive
{
public:
	std::string PrimitiveType;
	GLfloat Angle;
	glm::vec3 Position;
	glm::vec3 RotationAxis;
	glm::vec3 Scale;
	GLuint VAO, VBO, EBO, diffuse, specular;
	
	Primitive();
	~Primitive();
	
	void setPrimitive(std::string type, glm::vec3 position);
	void draw(Shader& lightingShader, glm::mat4& view, glm::mat4& projection, Camera& camera);

	void draw();
	
	
	void setPosition(glm::vec3 position);
	void setAngle(GLfloat angle);
	void setRotationAxis(glm::vec3 rotationAxis);
	void setScale(glm::vec3 scale);
	

	glm::vec3 getPosition();
	GLfloat getAngle();
	glm::vec3 getRotationAxis();
	glm::vec3 getScale();




};

#endif // !
