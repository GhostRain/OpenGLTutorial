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
	unsigned int planeVAO, planeVBO;
	unsigned int floorTexture;

	TexturesWindow()
	{
	}

	virtual void init()
	{
		glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		float planeVertices[] = {
			// positions            // normals         // texcoords
			10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		//setKeyCallback(cameraCallback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		floorTexture = TextureFromFile("wood.png", "E:/OpenGLTutorial/resources/textures");

		shader = new Shader("advance_lighting.vs", "advance_lighting.fs");
		shader->use();
		shader->setVec3("lightColor", lightColor);
		shader->setVec3("viewPos", camera->Position);
		shader->setVec3("lightPos", lightPos);

		//glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//shader->setMatrixVec4("model", glm::value_ptr(model));

		glm::mat4 view;
		view = camera->GetViewMatrix();
		shader->setMatrixVec4("view", glm::value_ptr(view));

		//Í¶Ó°¾ØÕó
		glm::mat4 projection;
		projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
		shader->setMatrixVec4("projection", glm::value_ptr(projection));

		shader->setInt("texture1", 0);

		glEnable(GL_DEPTH_TEST);
	}

	virtual void excute()
	{
		float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		camera->processInput(window, deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);

		shader->use();
		glm::mat4 view;
		view = camera->GetViewMatrix();
		shader->setMatrixVec4("view", glm::value_ptr(view));
		shader->setVec3("viewPos", camera->Position);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
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