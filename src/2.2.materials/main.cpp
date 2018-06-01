#include <MY/window.h>
#include <MY/shader_s.h>
#include <iostream>
#include <SOIL.h>
#include <MY/filesystem.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void cameraCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
class TexturesWindow :public Window
{
public:
	bool isDrag;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat yaw = -90.0f;
	GLfloat pitch;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Shader *lamp;
	glm::vec3 lightPos = glm::vec3(1.5f, 0.8f, 1.0f);
	GLuint LAMP_VAO;

	TexturesWindow()
	{

	}

	virtual void init()
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		GLuint VBO;

		//ͨ��ID����VBO����
		glGenBuffers(1, &VBO);
		//�󶨵�GL_ARRAY_BUFFERĿ����
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//�Ѷ������ݸ��Ƶ�GPU������
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		initVAO();


		//������������һ��Ҫ�ڶ�������д�뻺��֮��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glGenVertexArrays(1, &LAMP_VAO);
		glBindVertexArray(LAMP_VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		//������VAO��֮����ܰ�ȫ���VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		setKeyCallback(cameraCallback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		glEnable(GL_DEPTH_TEST);

		shader = new Shader("colors.vs", "colors.fs");
		shader->use();

		glm::mat4 model;
		model = glm::rotate(model, -45.0f * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		shader->setMatrixVec4("model", glm::value_ptr(model));

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader->setMatrixVec4("view", glm::value_ptr(view));

		//ͶӰ����
		glm::mat4 projection;
		projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
		shader->setMatrixVec4("projection", glm::value_ptr(projection));

		GLint objectColorLoc = glGetUniformLocation(shader->ID, "objectColor");
		GLint lightPosLoc = glGetUniformLocation(shader->ID, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shader->ID, "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(view));
		GLint matAmbientLoc = glGetUniformLocation(shader->ID, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(shader->ID, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(shader->ID, "material.specular");
		GLint matShineLoc = glGetUniformLocation(shader->ID, "material.shininess");

		GLint lightAmbientLoc = glGetUniformLocation(shader->ID, "light.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(shader->ID, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(shader->ID, "light.specular");

		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);// �����ǰ���������һ�㣬�����ῴ��������Ȼ
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(matShineLoc, 32.0f);

		lamp = new Shader("lamp.vs", "lamp.fs");
		lamp->use();
		glm::mat4 modelLamp;
		modelLamp = glm::translate(modelLamp, lightPos);
		modelLamp = glm::scale(modelLamp, glm::vec3(0.2f));
		lamp->setMatrixVec4("model", glm::value_ptr(modelLamp));
		lamp->setMatrixVec4("view", glm::value_ptr(view));
		lamp->setMatrixVec4("projection", glm::value_ptr(projection));
	}

	virtual void excute()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//������ɫ��
		shader->use();

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader->setMatrixVec4("view", glm::value_ptr(view));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lamp->use();
		lamp->setMatrixVec4("view", glm::value_ptr(view));
		glBindVertexArray(LAMP_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}

	void startDrag()
	{
		isDrag = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void stopDrag()
	{
		isDrag = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

private:

};
TexturesWindow *win;
void cameraCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	GLfloat cameraSpeed = 0.03f;
	if (key == GLFW_KEY_W)
		win->cameraPos += cameraSpeed * win->cameraFront;
	if (key == GLFW_KEY_S)
		win->cameraPos -= cameraSpeed * win->cameraFront;
	if (key == GLFW_KEY_A)
		win->cameraPos -= glm::normalize(glm::cross(win->cameraFront, win->cameraUp)) * cameraSpeed;
	if (key == GLFW_KEY_D)
		win->cameraPos += glm::normalize(glm::cross(win->cameraFront, win->cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (win->isDrag)
	{
		GLfloat xoffset = xpos - win->lastX;
		GLfloat yoffset = win->lastY - ypos;

		GLfloat sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		win->yaw += xoffset;
		win->pitch += yoffset;

		glm::vec3 front;
		front.x = cos(glm::radians(win->yaw)) * cos(glm::radians(win->pitch));
		front.y = sin(glm::radians(win->pitch));
		front.z = sin(glm::radians(win->yaw)) * cos(glm::radians(win->pitch));
		win->cameraFront = glm::normalize(front);
	}
	win->lastX = xpos;
	win->lastY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int key, int action, int mods)
{
	//����
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			win->startDrag();
		}
	}
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			win->stopDrag();
		}
	}
}

int main()
{
	win = new TexturesWindow();
	win->creat();
}