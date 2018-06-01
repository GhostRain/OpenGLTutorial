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
#include <map>

void cameraCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
class TexturesWindow :public Window
{
public:
	Camera * camera;
	unsigned int cubeTexture;
	unsigned int floorTexture;
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;
	unsigned int quadVAO, quadVBO;
	GLuint fbo;
	Shader *screenShader;
	GLuint texture;

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
		float transparentVertices[] = {
			5.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			5.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			5.0f, -1.0f,  3.0f,  1.0f,  1.0f,

			5.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			5.0f, -1.0f,  3.0f,  1.0f,  1.0f,
			5.0f,  1.0f,  3.0f,  1.0f,  0.0f
		};
		float quadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};
		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		cubeTexture = TextureFromFile("marble.jpg", "E:/OpenGLTutorial/resources/textures");
		floorTexture = TextureFromFile("metal.png", "E:/OpenGLTutorial/resources/textures");

		setKeyCallback(cameraCallback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		shader = new Shader("stencil_testing.vs", "stencil_testing.fs");
		screenShader = new Shader("framebuffers_screen.vs", "framebuffers_screen.fs");

		shader->use();
		shader->setInt("texture1", 0);

		//投影矩阵
		glm::mat4 projection;
		projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
		shader->setMatrixVec4("projection", glm::value_ptr(projection));

		screenShader->use();
		screenShader->setInt("screenTexture", 0);
		screenShader->setMatrixVec4("projection", glm::value_ptr(projection));

		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		//创建一个帧缓冲,至少加入一个附件（颜色、深度、模板缓冲）
		glGenFramebuffers(1, &fbo);
		//绑定到当前帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//创建一个帧缓冲的纹理附件
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//NULL作为纹理的data参数表示只分配内存，而不去填充它
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//把纹理附加到帧缓冲上
		//target：我们所创建的帧缓冲类型的目标（绘制、读取或两者都有）。
		//attachment：我们所附加的附件的类型。现在我们附加的是一个颜色附件。需要注意，最后的那个0是暗示我们可以附加1个以上颜色的附件。
		//textarget：你希望附加的纹理类型。
		//texture：附加的实际纹理。
		//level：Mipmap level。我们设置为0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		//创建一个帧缓冲的缓冲对象附件,用于深度、模板缓冲
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		//绑定缓冲对象
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//创建一个深度和模板渲染缓冲对象
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
		//把缓冲对象附加到帧缓冲上
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		//检查帧缓冲
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		//绑定为0来使默认帧缓冲被激活
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//删除帧缓冲对象
		//glDeleteFramebuffers(1, &fbo);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void excute()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view;
		view = camera->GetViewMatrix();

		//激活着色器
		shader->use();
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		model = glm::mat4();
		shader->setMatrixVec4("model", glm::value_ptr(model));
		shader->setMatrixVec4("view", glm::value_ptr(view));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		model = glm::mat4();
		shader->setMatrixVec4("model", glm::value_ptr(model));
		shader->setMatrixVec4("view", glm::value_ptr(view));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

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

		
		screenShader->use();
		screenShader->setMatrixVec4("model", glm::value_ptr(model));
		screenShader->setMatrixVec4("view", glm::value_ptr(view));
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
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