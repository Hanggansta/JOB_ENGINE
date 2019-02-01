#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include<stb/stb_image_write.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>




#include<vector>
#include<map>
#include "Shader.h"
#include "Texture.h"
#include "camera.h"
#include "Primitive.h"

#include "Material.h"
#include <iostream>


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
//void createframebuffer();


// settings

GLuint SCR_WIDTH ;
GLuint SCR_HEIGHT ;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool OpenGUI = false;
bool OpenSpotLight = false;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
GLfloat cutoff = 12.0f;
GLfloat outercutoff = 17.5f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 10.0f);

Texture diffuse;
Texture specular;
Texture blendTexture;

bool show_demo_window = false;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void imguisetup();
bool cameraMode = false;

Primitive cube;
Primitive lamp;
Primitive plane;
Primitive quad;

Material material;
Material M_stencil;


glm::vec3 lightDirection=glm::vec3(-0.2f, -1.0f, -0.3f);
glm::vec4 lightColor;
glm::vec3 M_ambient = glm::vec3(0.19225f, 0.19225f, 0.19225);
glm::vec3 M_diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
glm::vec3 M_specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);

std::vector<glm::vec3> windows
{
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};





int main()
{
    #pragma region Initialize
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	const char* glsl_version = "#version 440";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* glfwMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* glfwMode = glfwGetVideoMode(glfwMonitor);

	SCR_WIDTH = glfwMode->width;
	SCR_HEIGHT = glfwMode->height;
	//glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLEngine", glfwMonitor, NULL);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JOB_ENGINE", glfwMonitor, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();	
	//io.Fonts->AddFontFromFileTTF("c:/windows/Fonts/arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	

#pragma endregion 

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	
	//这会告诉OpenGL，只要一个片段的模板值等于(GL_EQUAL)参考值1，片段将会通过测试并被绘制，否则会被丢弃。
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	cube.setPrimitive("cube", glm::vec3(0.0f, 0.0f, 0.0f));	
	lamp.setPrimitive("cube", lightPos);
	plane.setPrimitive("plane",glm::vec3(0.0f,0.0f,0.0f) );
	//plane.setPrimitive("plane", glm::vec3(0.0f, 0.0f, 0.0f));
	plane.setScale(glm::vec3(5.0f));


	quad.setPrimitive("quad", glm::vec3(0.0f, 0.0f, 0.0f));

	Shader lightingShader("material", "material.vert", "material.frag");//材质的
	Shader lampShader("lamp", "lamp.vert", "lamp.frag");//灯的
	Shader LightShader("all", "AllLight.vert", "AllLight.frag"); //多光源的
	Shader shader("stencil", "stencil.vert", "stencil.frag"); // 模板测试第一次渲染
	Shader shaderSingleColor("single_color", "stencil.vert", "stencil_single_color.frag");//模板测试第二次渲染
	Shader blendingShader("blending", "blending.vert", "blending.frag"); //混合
	Shader glassCubeShader("glass", "glass.vert", "glass.frag");

	diffuse.setTexture("container2.png","diffuse",true);
	specular.setTexture("container2_specular.png", "specular", true);
	blendTexture.setTexture("window.png", "window", true);
	
   	material.addTexture("material.diffuse", diffuse);
	material.addTexture("material.specular", specular);
	material.setShader(LightShader);
	
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < windows.size(); i++)
	{
		float distance = glm::length(camera.Position - windows[i]);
		sorted[distance] = windows[i];
	}	





	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	
		processInput(window);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		glfwPollEvents();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
      //  禁止模板缓冲写入
        #pragma region GOLD_SILVER
		lightingShader.Use();		
		lightingShader.setVec3("light.position", camera.Position);  
		// light properties
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f); // low influence

		lightingShader.setVec3("light.ambient", ambientColor);
		lightingShader.setVec3("light.diffuse", diffuseColor);
		lightingShader.setVec3("light.specular",glm::vec3(1.0f, 1.0f, 1.0f));

		// material properties
		lightingShader.setVec3("material.ambient", M_ambient);
		lightingShader.setVec3("material.diffuse", M_diffuse);
		lightingShader.setVec3("material.specular",M_specular); // specular lighting doesn't have full effect on this object's material
		lightingShader.setFloat("material.shininess", 51.2f);

		cube.setPosition(glm::vec3(-4.0f,0.0f,0.0f));
		cube.draw(lightingShader, view, projection, camera);
#pragma endregion	

        #pragma region 10_CUBES
		LightShader.Use();
		LightShader.setBool("openSpotLight", OpenSpotLight);
		LightShader.setVec3("viewPos", camera.Position);
		LightShader.setFloat("material.shininess", 32.0f);
		LightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		LightShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		LightShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		LightShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		LightShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		LightShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		LightShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		LightShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		LightShader.setFloat("pointLights[0].constant", 1.0f);
		LightShader.setFloat("pointLights[0].linear", 0.09f);
		LightShader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		LightShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		LightShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		LightShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		LightShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		LightShader.setFloat("pointLights[1].constant", 1.0f);
		LightShader.setFloat("pointLights[1].linear", 0.09f);
		LightShader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		LightShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		LightShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		LightShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		LightShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		LightShader.setFloat("pointLights[2].constant", 1.0f);
		LightShader.setFloat("pointLights[2].linear", 0.09f);
		LightShader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		LightShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		LightShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		LightShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		LightShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		LightShader.setFloat("pointLights[3].constant", 1.0f);
		LightShader.setFloat("pointLights[3].linear", 0.09f);
		LightShader.setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		LightShader.setVec3("spotLight.position", camera.Position);
		LightShader.setVec3("spotLight.direction", camera.Front);
		LightShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
		LightShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		LightShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		LightShader.setFloat("spotLight.constant", 1.0f);
		LightShader.setFloat("spotLight.linear", 0.09);
		LightShader.setFloat("spotLight.quadratic", 0.032);
		LightShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(cutoff)));
		LightShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outercutoff)));
		material.renderToShader();

		for (GLuint i = 0; i <4; i++)
		{
			cube.setPosition(cubePositions[i]);
			cube.draw(LightShader, view, projection, camera);
		}	
		for (GLuint i = 0; i < 4; i++)
		{
			lamp.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
			lamp.setPosition(pointLightPositions[i]);
			lamp.draw(lampShader, view, projection, camera);
		}
		plane.draw(LightShader, view, projection, camera);
#pragma endregion	

        
        #pragma region Stencil
		//模板测试
		//floor
		M_stencil.clear();
		M_stencil.addTexture("texture1", diffuse);
		M_stencil.setShader(shader);
		M_stencil.renderToShader();		
		//plane.draw(shader, view, projection, camera);
		
		//1st
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//箱子的每个片段都更新为1
		glStencilMask(0xFF);//启用模板缓冲写入	
		cube.setPosition(glm::vec3(-1.0f, 0.0f, -1.0f));
		cube.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		cube.draw(shader, view, projection, camera);
		cube.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		cube.setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
		cube.draw(shader, view, projection, camera);
		
	
		//2st 目前箱子的模板值都是1 
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //只绘制模板值不为1的部分
		glStencilMask(0x00);  //禁止模板缓冲写入 
		glDisable(GL_DEPTH_TEST); //禁止深度测试
		float scale = 1.01; 

	
		M_stencil.setShader(shaderSingleColor);
		M_stencil.renderToShader();
		cube.setPosition(glm::vec3(-1.0f, 0.0f, -1.0f));
		cube.setScale(glm::vec3(scale, scale, scale));
		cube.draw(shaderSingleColor, view, projection, camera);
		
		cube.setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
		cube.setScale(glm::vec3(scale, scale, scale));
		cube.draw(shaderSingleColor, view, projection, camera);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);//重新启用深度测试 
#pragma endregion
       
        #pragma region blending
		M_stencil.clear();
		M_stencil.setShader(blendingShader);
		M_stencil.addTexture("texture1", blendTexture);
		M_stencil.renderToShader();
		/*for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			quad.setPosition(it->second);
			quad.draw(blendingShader, view, projection, camera);
		}*/
		quad.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		quad.setScale(glm::vec3(5.0f));

		quad.draw(blendingShader, view, projection, camera);

#pragma endregion


   

         
  

        
		




		glStencilFunc(GL_ALWAYS, 1, 0xFF);//解决了GUI的问题
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();		
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		if (OpenGUI)
			imguisetup();

		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	
	}

	glDeleteVertexArrays(1, &cube.VAO);
	glDeleteBuffers(1, &cube.VBO);
	glDeleteVertexArrays(1, &lamp.VAO);
	glDeleteBuffers(1, &lamp.VBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		OpenGUI = true;

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 

	lastX = xpos;
	lastY = ypos;
	if(cameraMode)
		camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void imguisetup()
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Super Power Egnine");                         

	ImGui::Text("This is my fucking 3D Engine,HAHA!!");              
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Mother fucker!");
	
	ImGui::Checkbox("Demo Window", &show_demo_window);   //这里GUI的官方范例

	ImGui::ColorEdit3("clear color", (float*)&clear_color); 
	ImGui::SliderFloat("CameraPosition.x", &camera.Position.x, 0.0f, 100.0f);
	ImGui::SliderFloat("CameraPosition.y", &camera.Position.y, 0.0f, 100.0f);
	ImGui::SliderFloat("CameraPosition.z", &camera.Position.z, 0.0f, 100.0f);

	ImGui::Checkbox("Material Edit", &show_another_window);
	if (show_another_window)
	{	
		ImGui::Begin("Material Edit");
		if (ImGui::Button("gold"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			M_ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
			M_diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
			M_specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
		}ImGui::SameLine();
		if (ImGui::Button("silver", ImVec2(0.0f, 0.0f)))
		{
			M_ambient = glm::vec3(0.19225f, 0.19225f, 0.19225);
			M_diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
			M_specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
		}ImGui::SameLine();
		ImGui::End();
	}
	if (ImGui::Button("Open SpotLight"))
	{
		OpenSpotLight = true;	
	}ImGui::SameLine();
	if (ImGui::Button("Close SpotLight"))
	{
		OpenSpotLight = false;
	}
	//ImGui::SameLine();
	ImGui::SliderFloat("SpotLight_cutoff", &cutoff, 1.0f, 100.0f);
	ImGui::SliderFloat("SpotLight_outercutoff", &outercutoff, 1.0f, 100.0f);



	ImGui::Text("Time = %f", glfwGetTime());

	ImGui::Text("Camera Position= %.3f %.3f %.3f", camera.Position.x, camera.Position.y, camera.Position.z);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Bullet();	
	ImGui::End();
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		cameraMode = true;
	else
		cameraMode = false;
}

//void createframebuffer()
//{
//	glGenFramebuffers(1, &framebuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//	// create a color attachment texture
//	//GLuint textureColorbuffer;
//	glGenTextures(1, &textureColorbuffer);
//	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
//	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
//	GLuint rbo;
//	glGenRenderbuffers(1, &rbo);
//	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
//	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}







