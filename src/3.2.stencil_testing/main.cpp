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
	Camera *camera;
	Shader *shaderSingleColor;
	unsigned int cubeTexture;
	unsigned int floorTexture;
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;
	unsigned int stencilVAO, stencilVBO;

	TexturesWindow()
	{

	}

	virtual void init()
	{
		float cubeVertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		float planeVertices[] = {
			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
		GLfloat stencilVertices[] = {
			-0.5f, -0.5f, 0.5f, // A
			-0.5f, 0.5f, 0.5f,	// B
			-1.5f, 0.5f, 0.5f,  // C

			-1.5f, 0.5f, 0.5f,  // C
			-1.5f, -0.5f, 0.5f, // D
			-0.5f, -0.5f, 0.5f  // A
		};
		// cube VAO
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);
		// plane VAO
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		glGenVertexArrays(1, &stencilVAO);
		glGenBuffers(1, &stencilVBO);
		glBindVertexArray(stencilVAO);
		glBindBuffer(GL_ARRAY_BUFFER, stencilVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(stencilVertices), &stencilVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);

		cubeTexture = TextureFromFile("marble.jpg", "E:/OpenGLTutorial/resources/textures");
		floorTexture = TextureFromFile("metal.png", "E:/OpenGLTutorial/resources/textures");

		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		setKeyCallback(cameraCallback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		shader = new Shader("stencil_testing.vs", "stencil_testing.fs");
		shaderSingleColor = new Shader("stencil_testing.vs", "stencil_single_color.fs");

		shader->use();
		shader->setInt("texture1", 0);

		//ͶӰ����
		glm::mat4 projection;
		projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
		shader->setMatrixVec4("projection", glm::value_ptr(projection));

		shaderSingleColor->use();
		shaderSingleColor->setMatrixVec4("projection", glm::value_ptr(projection));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	virtual void excute()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view;
		view = camera->GetViewMatrix();

		glDisable(GL_DEPTH_TEST);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glBindVertexArray(stencilVAO);
		shaderSingleColor->use();
		shaderSingleColor->setMatrixVec4("view", glm::value_ptr(view));
		shaderSingleColor->setMatrixVec4("model", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF);

		//������ɫ��
		shader->use();
		
		//�ر�ģ�建��д��
		//glStencilMask(0x00);
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		model = glm::mat4();
		shader->setMatrixVec4("model", glm::value_ptr(model));
		shader->setMatrixVec4("view", glm::value_ptr(view));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		////����ģ�建��д�뺯��
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		////����ģ�建��д��
		//glStencilMask(0xFF);

		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		shader->setMatrixVec4("view", glm::value_ptr(view));

		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader->setMatrixVec4("model", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader->setMatrixVec4("model", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);  

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);

		//shaderSingleColor->use();
		//float scale = 1.1;
		//// cubes
		//glBindVertexArray(cubeVAO);
		//glBindTexture(GL_TEXTURE_2D, cubeTexture);
		//model = glm::mat4();
		//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//shaderSingleColor->setMatrixVec4("model", glm::value_ptr(model));
		//shaderSingleColor->setMatrixVec4("view", glm::value_ptr(view));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//model = glm::mat4();
		//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//shaderSingleColor->setMatrixVec4("model", glm::value_ptr(model));
		//shaderSingleColor->setMatrixVec4("view", glm::value_ptr(view));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}

private:

};
TexturesWindow *win;
void cameraCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	win->camera->cameraCallback(window, key, scancode, action, mode);
}

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