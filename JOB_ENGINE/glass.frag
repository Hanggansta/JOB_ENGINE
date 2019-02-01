#version 440 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform samplerCube env;

void main()
{
    vec3 I=normalize(Position-viewPos);
	vec3 R=reflect(I,normalize(Normal));
	FragColor=vec4(texture(env,R).rgb,1.0);


}