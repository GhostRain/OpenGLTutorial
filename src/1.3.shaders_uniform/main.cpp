// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

GLfloat vertices[] = {
	// 位置              // 颜色
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};
GLuint VBO;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
GLuint VAO;
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = color;"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"in vec4 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = ourColor;\n"
"}\n\0";

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//创建顶点缓冲对象VBO，创建顶点数组对象VAO
void creatVBO()
{
	//通过ID生成VBO对象
	glGenBuffers(1,&VBO);
	//绑定到GL_ARRAY_BUFFER目标上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把顶点数据复制到GPU缓存里
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//解析顶点数据一定要在顶点数据写入缓存之后
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//必须在VAO绑定之后才能安全解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//编译着色器
void compileVertexShader()
{
	//通过ID生成顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//把顶点着色器代码附到着色器上
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译着色器
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//通过ID生成片段着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//把片段着色器代码附到着色器上
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//编译着色器
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

//链接着色器
void linkShader()
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

int main()
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);

	//初始化GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	//从GLFW中获取视口的维度
	glfwGetFramebufferSize(window, &width, &height);
	//设置窗口的维度
	glViewport(0, 0, width, height);
	//注册GLFW的按键回调
	glfwSetKeyCallback(window, key_callback);

	compileVertexShader();
	linkShader();
	//creatVAO();
	creatVBO();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		// 检查事件
		glfwPollEvents();
		// 渲染指令
		//设置清空屏幕所用的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清空屏幕的颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT);

		/*GLfloat timeValue = glfwGetTime();
		GLfloat colorValue = (sin(timeValue) / 2) + 0.5;
		GLuint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");*/

		//激活着色器
		glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 1.0f, colorValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		// 交换缓冲
		glfwSwapBuffers(window);
	}
	//释放GLFW分配的内存
	glfwTerminate();
	return 0;
}