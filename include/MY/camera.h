#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        //updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
		return glm::lookAt(Position, Position + Front, WorldUp);
    }

	void startDrag()
	{
		isDrag = true;
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void stopDrag()
	{
		isDrag = false;
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void cameraCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		GLfloat cameraSpeed = 0.1f;
		if (key == GLFW_KEY_W)
			Position += cameraSpeed * Front;
		if (key == GLFW_KEY_S)
			Position -= cameraSpeed * Front;
		if (key == GLFW_KEY_A)
			Position -= glm::normalize(glm::cross(Front, WorldUp)) * cameraSpeed;
		if (key == GLFW_KEY_D)
			Position += glm::normalize(glm::cross(Front, WorldUp)) * cameraSpeed;
	}

	void processInput(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		GLfloat cameraSpeed = 2.0f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Position += cameraSpeed * Front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Position -= cameraSpeed * Front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Position -= glm::normalize(glm::cross(Front, WorldUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Position += glm::normalize(glm::cross(Front, WorldUp)) * cameraSpeed;
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (isDrag)
		{
			GLfloat xoffset = xpos - lastX;
			GLfloat yoffset = lastY - ypos;

			GLfloat sensitivity = 0.05f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			updateCameraVectors();
		}
		lastX = xpos;
		lastY = ypos;
	}

	void mouse_button_callback(GLFWwindow* window, int key, int action, int mods)
	{
		//°´ÏÂ
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_MOUSE_BUTTON_LEFT)
			{
				startDrag();
			}
		}
		if (action == GLFW_RELEASE)
		{
			if (key == GLFW_MOUSE_BUTTON_LEFT)
			{
				stopDrag();
			}
		}
	}

private:
	bool isDrag = false;
	GLfloat lastX;
	GLfloat lastY;
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
    }
};
#endif