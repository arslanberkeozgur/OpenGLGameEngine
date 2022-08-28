#pragma once

#include "Camera.h"
#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "GLObject.h"
#include "Windows.h"

class GLManager
{
public:
	GLManager() 
	{
		// Allow modern extension access
		glewExperimental = GL_TRUE;

		// For some kind of (poor) time interval consistency.
		deltaTime = 0.0f;
		lastTime = 0.0f;
		
		mainWindow = nullptr;
		shader = new Shader();

		mainCamera = new ActiveCamera(glm::vec3(0.0f, 0.0f, 00.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, true, 5.0f, 100.0f);
		listOfCameras.push_back(std::move(std::unique_ptr<Camera>(mainCamera)));
		activeCamera = mainCamera;

	}
	~GLManager()
	{
		Destroy();
	}

	// Handling cameras.
	GLuint CreateCamera(glm::vec3 pos, glm::vec3 up, GLfloat yaw, GLfloat pitch, bool active)
	{
		// Generate and push back.
		std::unique_ptr<Camera> cam(new Camera(pos, up, yaw, pitch, active));
		listOfCameras.push_back(std::move(cam));
		
		// Set as active camera
		if (active) activeCamera = cam.get();
		
		// Return index
		return listOfCameras.size() - 1;
	}
	void SetActiveCam(Camera* cam)
	{
		// Set all the rest as inactive.
		for (auto& cam : listOfCameras)
			if (cam != nullptr)
				cam->SetActive(false);

		// Set active.
		cam->SetActive(true);
		activeCamera = cam;
	}
	Camera* GetCamWithIndex(GLuint index)
	{
		// Iterate from the beginning.
		auto iterator = listOfCameras.cbegin();
		std::advance(iterator, index);
		return (*iterator).get();
	}

	// Handling Objects.
	GLuint CreateObject(Geometry geometry)
	{
		// Generate, push back and return index.
		std::unique_ptr<GLObject> obj(new GLObject(listOfObjects.size(), geometry));
		listOfObjects.push_back(std::move(obj));
		return listOfObjects.size() - 1;
	}
	GLObject* GetObjectWithIndex(GLuint index)
	{
		auto iterator = listOfObjects.cbegin();
		std::advance(iterator, index);
		return (*iterator).get();
	}
	void KillObject(GLObject* obj)
	{
		// Removing from the list of objects calls its destructor, since we are using unique pointers.
		std::remove_if(listOfObjects.begin(), listOfObjects.end(), [obj](std::unique_ptr<GLObject>& ob) { return ob.get() == obj; });
	}

	bool GetKey(GLint key) { return mainWindow->getKeys()[key]; };

	GLuint GetObjectCount() { return listOfObjects.size(); };
	GLuint GetCamCount() { return listOfCameras.size(); };
	
	// User must override these.
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(GLdouble deltaTime) = 0;

	void Run(GLint width, GLint height, GLfloat R, GLfloat G, GLfloat B)
	{
		mainWindow = new GLWindow(width, height);
		mainWindowColor = glm::vec3(R, G, B);
		mainWindow->Initialize();
		if (!Loop())
			Destroy();
	}
private:
	GLWindow* mainWindow;
	glm::vec3 mainWindowColor;
	Shader* shader;
	
	ActiveCamera* mainCamera;
	Camera* activeCamera;  // Shorthand for reaching the active camera among others.
	std::list<std::unique_ptr<Camera>> listOfCameras;
	std::list<std::unique_ptr<GLObject>> listOfObjects;

	GLdouble deltaTime;
	GLdouble lastTime;

	// Main loop of the animations.
	bool Loop()
	{
		// Handle shader
		static const char* fShader = "Shaders/shader.frag";
		static const char* vShader = "Shaders/shader.vert";
		shader->CreateFromFiles(vShader, fShader);

		// Get uniform variable IDs.
		GLuint uniformModel = shader->GetModelLocation();
		GLuint uniformProjection = shader->GetProjectionLocation();
		GLuint uniformView = shader->GetViewLocation();	
		
		// This is where user creates resources.
		if (!OnUserCreate())
			return false;
		
		// Standart 3d perspective projection.
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(GLfloat)mainWindow->getBufferWidth() / mainWindow->getBufferHeight(), 0.1f, 500.0f);

		double seconds = glfwGetTime();
		int nbFrames = 0;

		while (!mainWindow->getShouldClose())
		{
			// Handle time
			deltaTime = (GLdouble)glfwGetTime();
			deltaTime = deltaTime - lastTime;
			lastTime = (GLdouble)glfwGetTime();

			// Measure speed
			double currentTime = glfwGetTime();
			nbFrames++;
			if (currentTime - seconds >= 1.0) { // If last prinf() was more than 1 sec ago
				// printf and reset timer
				printf("%f ms/frame\n", 1000.0 / double(nbFrames));
				nbFrames = 0;
				seconds += 1.0;
			}

			if (mainCamera->GetActive())
			{
				mainCamera->keyControl(mainWindow->getKeys(), deltaTime);
				mainCamera->MouseControl(mainWindow->GetMouseXChange(), mainWindow->GetMouseYChange(), deltaTime, 2);
			}

			// This is where the user creates per loop events.
			if (!OnUserUpdate(deltaTime)) return false;

			// Everything under this part is about rendering.
			shader->UseShader();
			
				// Assign calculated uniform variables.
				glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(activeCamera->CalculateViewMatrix()));

				for (auto& object : listOfObjects)
				{
					if (object != nullptr)
					{
						object->Update();
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(object->GetModelMatrix()));
						object->Render();
					}
				}

			shader->DropShader();
			
			mainWindow->SwapBuffers();
			mainWindow->PollEvents();
			mainWindow->Clear(mainWindowColor.x, mainWindowColor.y, mainWindowColor.z);
		}
		return true;
	}
	void Destroy()
	{
		listOfObjects.clear();
		listOfCameras.clear();
		delete shader;
		delete mainWindow;
	}
};