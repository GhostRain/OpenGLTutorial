#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <MY/shader_s.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

class Window 
{
public:
	Shader *shader;
	GLuint VAO;
	GLFWwindow* window;
	int width = 800;
	int height = 600;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	Window()
	{
		
	}

	void creat()
	{
		//��ʼ��GLFW
		glfwInit();
		//���汾��(Major)�ʹΰ汾��(Minor)����Ϊ3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//ʹ�õ��Ǻ���ģʽ,ʹ�þɰ溯�����ᵼ��invalid operation(��Ч����)�Ĵ�����ʾ
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//�������û��������ڵĴ�С
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//��������
		window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
		glfwMakeContextCurrent(window);

		//��ʼ��GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}

		//��GLFW�л�ȡ�ӿڵ�ά��
		glfwGetFramebufferSize(window, &width, &height);
		//���ô��ڵ�ά��
		glViewport(0, 0, width, height);
		//ע��GLFW�İ����ص�
		glfwSetKeyCallback(window, key_callback);

		init();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		while (!glfwWindowShouldClose(window))
		{
			// ����¼�
			glfwPollEvents();
			// ��Ⱦָ��
			excute();
			// ��������
			glfwSwapBuffers(window);
		}
		//�ͷ�GLFW������ڴ�
		glfwTerminate();
	}

	void setKeyCallback(GLFWkeyfun callback)
	{
		glfwSetKeyCallback(window, callback);
	}

	void initVAO()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	}
protected:
	virtual void excute() {};
	virtual void init() {};
private:
};