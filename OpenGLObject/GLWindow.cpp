#include "GLWindow.h"
#include <windows.h>

GLWindow::GLWindow()
{
	width = 800;
	height = 600;

	// Reset the memory for keys
	memset(keys, 0, sizeof(bool) * 1024);

	// Mouse hasn't moved yet. (In order to prevent the flickering in the beginning)
	firstMouse = true;
	xChange = 0.0f;
	yChange = 0.0f;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	// Reset the memory for keys
	memset(keys, 0, sizeof(bool) * 1024);

	// Mouse hasn't moved yet. (In order to prevent the flickering in the beginning)
	firstMouse = true;
	xChange = 0.0f;
	yChange = 0.0f;
}

int GLWindow::Initialize()
{
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW Windows Properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the current context
	glfwMakeContextCurrent(mainWindow);

	// Handle Key + Mouse Input
	CreateCallbacks();
	
	// Disable cursor movement
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Give access of the mainWindow to static callback functions. 
	// This is because mainWindow is an instant attribute while functions are owned by class.
	glfwSetWindowUserPointer(mainWindow, this);

	// In order to get consistent window positions on screen
	glfwSetWindowPos(mainWindow, 100, 100);
}

void GLWindow::Clear(GLfloat r, GLfloat g, GLfloat b)
{
	// Clear the window
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLfloat GLWindow::GetMouseXPos()
{
	return lastX;
}

GLfloat GLWindow::GetMouseYPos()
{
	return lastY;
}

GLfloat GLWindow::GetMouseXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat GLWindow::GetMouseYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void GLWindow::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, key_callback);
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
}

void GLWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Get access to mainWindow.
	GLWindow* glWindow = (GLWindow*)glfwGetWindowUserPointer(window);
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			glWindow->keys[key] = true;
		else if (action == GLFW_RELEASE)
			glWindow->keys[key] = false;
	}
}

void GLWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Get access to mainWindow.
	GLWindow* glWindow = (GLWindow*)glfwGetWindowUserPointer(window);

	if (glWindow->firstMouse)
	{
		glWindow->lastX = xpos;
		glWindow->lastY = ypos;
		glWindow->firstMouse = false;
	}

	glWindow->xChange = xpos - glWindow->lastX;
	glWindow->yChange = glWindow->lastY - ypos;

	glWindow->lastX = xpos;
	glWindow->lastY = ypos;
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
