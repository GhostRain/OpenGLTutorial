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
	Shader *shaderPlanet;
	Shader *shaderRock;
	unsigned int cubeVAO, cubeVBO;
	Model *planet;
	Model *rock;
	GLuint amount = 1000;
	glm::mat4* modelMatrices;
	GLfloat displacementArr[1000];
	GLfloat angleArr[1000];
	GLfloat radius = 50.0;

	TexturesWindow()
	{
	}

	virtual void init()
	{
		camera = new Camera(glm::vec3(0.0f, 10.0f, 70.0f));
		setKeyCallback(cameraCallback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		planet = new Model("E:/OpenGLTutorial/resources/objects/planet/planet.obj");
		rock = new Model("E:/OpenGLTutorial/resources/objects/rock/rock.obj");

		modelMatrices = new glm::mat4[amount];
		srand(glfwGetTime()); // initialize random seed

		GLfloat offset = 2.5f;
		for (GLuint i = 0; i < amount; i++)
		{
			glm::mat4 model;
			// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
			GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
			GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat x = sin(angle) * radius + displacement;
			displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat y = displacement * 0.4f; // y value has smaller displacement
			displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));
			// 2. Scale: Scale between 0.05 and 0.25f
			GLfloat scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));
			// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
			GLfloat rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
			// 4. Now add to list of matrices
			modelMatrices[i] = model;
			displacementArr[i] = displacement;
			angleArr[i] = angle;
		}

		for (GLuint i = 0; i < rock->meshes.size(); i++)
		{
			GLuint VAO = rock->meshes[i].VAO;
			// Vertex Buffer Object
			GLuint buffer;
			glBindVertexArray(VAO);
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
			// Vertex Attributes
			GLsizei vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}

		shaderPlanet = new Shader("model_loading.vs", "model_loading.fs");
		shaderPlanet->use();

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		shaderPlanet->setMatrixVec4("model", glm::value_ptr(model));

		glm::mat4 view;
		view = camera->GetViewMatrix();
		shaderPlanet->setMatrixVec4("view", glm::value_ptr(view));

		//Í¶Ó°¾ØÕó
		glm::mat4 projection;
		projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
		shaderPlanet->setMatrixVec4("projection", glm::value_ptr(projection));

		shaderRock = new Shader("instancing_shader.vs", "instancing_shader.fs");
		shaderRock->use();
		shaderRock->setMatrixVec4("projection", glm::value_ptr(projection));

		glEnable(GL_DEPTH_TEST);
	}

	virtual void excute()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderPlanet->use();

		glm::mat4 view;
		view = camera->GetViewMatrix();
		shaderPlanet->setMatrixVec4("view", glm::value_ptr(view));

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		shaderPlanet->setMatrixVec4("model", glm::value_ptr(model));

		planet->Draw(shaderPlanet);

		shaderRock->use();
		shaderRock->setMatrixVec4("view", glm::value_ptr(view));
		for (GLuint i = 0; i < rock->meshes.size(); i++)
		{
			glBindVertexArray(rock->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].vertices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}
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