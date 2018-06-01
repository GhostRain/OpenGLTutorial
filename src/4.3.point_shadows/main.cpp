#include <MY/window.h>
#include <MY/shader_s.h>
#include <iostream>
#include <SOIL.h>
#include <MY/filesystem.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <MY/model.h>
#include <MY/camera.h>

void cameraCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
class TexturesWindow :public Window
{
public:
	Camera * camera;
	//���������ͼ�Ľ�����Ϊ1024
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int cubeVAO, cubeVBO;
	unsigned int floorTexture;
	GLuint depthCubeMap;
	GLuint depthMapFBO;
	//������������
	Shader *cubeShader;
	//���������Ⱦ�����ͼ
	Shader *depthMapShader;
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat near_plane = 1.0f, far_plane = 25.0f;

	TexturesWindow()
	{
		width = 1280;
		height = 720;
	}

	virtual void init()
	{
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		float cubeVertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		//������������ͼ
		floorTexture = TextureFromFile("wood.png", "E:/OpenGLTutorial/resources/textures");

		//����һ��֡����
		glGenFramebuffers(1, &depthMapFBO);
		//�󶨵���ǰ֡����
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		//�����������ͼ
		glGenTextures(1, &depthCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		//�����ʽָ��ΪGL_DEPTH_COMPONENT,ֻ��Ҫ���ֵ
		for (GLuint i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			//glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//�������ӵ�֡������
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
		//����OpenGL���ǲ�ʹ���κ���ɫ���ݽ�����Ⱦ
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		//���֡����
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		cubeShader = new Shader("cube.vs", "cube.fs");
		cubeShader->use();
		cubeShader->setFloat("far_plane", far_plane);
		
		//ͶӰ����
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 1000.0f);
		cubeShader->setMatrixVec4("projection", glm::value_ptr(projection));
		
		cubeShader->setInt("diffuseTexture", 0);
		cubeShader->setInt("shadowMap", 1);

		depthMapShader = new Shader("shadow_mapping.vs", "shadow_mapping.fs","shadow_mapping.gs");
		depthMapShader->use();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void renderCube(Shader *shader)
	{
		glm::mat4 model;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader->use();
		//shader->setMat4("view", camera->GetViewMatrix());

		//���
		glDisable(GL_CULL_FACE);
		shader->setInt("reverse_normals", 1);
		model = glm::scale(model, glm::vec3(5.0f));
		renderCude(shader, model);

		glEnable(GL_CULL_FACE);
		shader->setInt("reverse_normals", 0);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		renderCude(shader, model);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.75f));
		renderCude(shader, model);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		renderCude(shader, model);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
		model = glm::scale(model, glm::vec3(0.5f));
		renderCude(shader, model);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.75f));
		renderCude(shader, model);
	}

	void renderCude(Shader *shader,glm::mat4 model)
	{
		shader->setMatrixVec4("model", glm::value_ptr(model));
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void ConfigureShaderAndMatrices()
	{
		GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		depthMapShader->use();
		for (unsigned int i = 0; i < 6; ++i)
			depthMapShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		depthMapShader->setFloat("far_plane", far_plane);
		depthMapShader->setVec3("lightPos", lightPos);
	}

	virtual void excute()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera->processInput(window, deltaTime);

		lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;

		//һ��Ҫ�ı䴰�ڳߴ�����Ӧ��Ӱ��ͼ�Ľ����ȣ�������Ⱦ�����Ļ᲻����
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//��Ⱦ��֡����
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//���������޳�,��ֹ����ʧ��
		//glCullFace(GL_FRONT);

		//��Ⱦ�����ͼ
		ConfigureShaderAndMatrices();
		renderCube(depthMapShader);

		//�ر������޳�
		//glCullFace(GL_BACK);
		//�رյ�֡����
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, width, height);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		cubeShader->use();
		//ConfigureShaderAndMatrices(cubeShader);
		cubeShader->setMat4("view", camera->GetViewMatrix());
		cubeShader->setVec3("viewPos", camera->Position);
		cubeShader->setVec3("lightPos", lightPos);
		renderCube(cubeShader);
	}

private:

};
TexturesWindow *win;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	win->camera->mouse_callback(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int key, int action, int mods)
{
	win->camera->mouse_button_callback(window, key, action, mods);
}

int main()
{
	win = new TexturesWindow();
	win->creat();
}