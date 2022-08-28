#pragma once

#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class GLWindow
{
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getKeys() { return keys; }
	GLfloat GetMouseXPos();
	GLfloat GetMouseYPos();
	GLfloat GetMouseXChange();
	GLfloat GetMouseYChange();

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }
	void Clear(GLfloat r, GLfloat g, GLfloat b);
	void PollEvents() { glfwPollEvents(); };

	~GLWindow();
private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLdouble lastX;
	GLdouble lastY;
	GLdouble xChange;
	GLdouble yChange;

	bool firstMouse;

	void CreateCallbacks();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};