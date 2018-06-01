// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

GLfloat vertices[] = {
	// λ��              // ��ɫ
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
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
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//�������㻺�����VBO�����������������VAO
void creatVBO()
{
	//ͨ��ID����VBO����
	glGenBuffers(1,&VBO);
	//�󶨵�GL_ARRAY_BUFFERĿ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�Ѷ������ݸ��Ƶ�GPU������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//������������һ��Ҫ�ڶ�������д�뻺��֮��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//������VAO��֮����ܰ�ȫ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//������ɫ��
void compileVertexShader()
{
	//ͨ��ID���ɶ�����ɫ��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//�Ѷ�����ɫ�����븽����ɫ����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//������ɫ��
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//ͨ��ID����Ƭ����ɫ��
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//��Ƭ����ɫ�����븽����ɫ����
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//������ɫ��
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

//������ɫ��
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(window);

	//��ʼ��GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	//��GLFW�л�ȡ�ӿڵ�ά��
	glfwGetFramebufferSize(window, &width, &height);
	//���ô��ڵ�ά��
	glViewport(0, 0, width, height);
	//ע��GLFW�İ����ص�
	glfwSetKeyCallback(window, key_callback);

	compileVertexShader();
	linkShader();
	//creatVAO();
	creatVBO();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		// ����¼�
		glfwPollEvents();
		// ��Ⱦָ��
		//���������Ļ���õ���ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//�����Ļ����ɫ����
		glClear(GL_COLOR_BUFFER_BIT);

		/*GLfloat timeValue = glfwGetTime();
		GLfloat colorValue = (sin(timeValue) / 2) + 0.5;
		GLuint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");*/

		//������ɫ��
		glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 1.0f, colorValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		// ��������
		glfwSwapBuffers(window);
	}
	//�ͷ�GLFW������ڴ�
	glfwTerminate();
	return 0;
}