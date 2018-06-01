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
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
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
		//初始化GLFW
		glfwInit();
		//主版本号(Major)和次版本号(Minor)都设为3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//使用的是核心模式,使用旧版函数将会导致invalid operation(无效操作)的错误提示
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//不允许用户调整窗口的大小
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//创建窗口
		window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		//通知GLFW将我们窗口的上下文设置为当前线程的主上下文
		glfwMakeContextCurrent(window);

		//初始化GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}

		//从GLFW中获取视口的维度
		glfwGetFramebufferSize(window, &width, &height);
		//设置窗口的维度
		glViewport(0, 0, width, height);
		//注册GLFW的按键回调
		glfwSetKeyCallback(window, key_callback);

		init();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		while (!glfwWindowShouldClose(window))
		{
			// 检查事件
			glfwPollEvents();
			// 渲染指令
			excute();
			// 交换缓冲
			glfwSwapBuffers(window);
		}
		//释放GLFW分配的内存
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