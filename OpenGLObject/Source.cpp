#include "GLManager.h"
#include "Windows.h"

extern "C" { _declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001; }

class Test : public GLManager
{
public:
	Test() {}
	~Test() {}
	
	bool OnUserCreate()
	{
		for (int i = 0; i < 100; i++)
		{
			GLuint index = CreateObject(CUBE);
			GetObjectWithIndex(index)->SetPosition(5 * i, 0.0f, 0.0f);
			GetObjectWithIndex(index)->SetVelocity((GLfloat)(rand() % 100) / 10000.0f, (GLfloat)(rand() % 100) / 10000.0f, (GLfloat)(rand() % 100) / 10000.0f);
			GetObjectWithIndex(index)->SetAngularVelocity((GLfloat)(rand() % 100) / 100.0f, 0.0f, 0.0f);
		}

		CreateCamera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, false);
		
	    followIndex = 0;
		followAngle = 0.0f;

		return true;
	}

	bool OnUserUpdate(GLdouble deltaTime)
	{
		if (GetKey(GLFW_KEY_0))
			SetActiveCam(GetCamWithIndex(0));
		if (GetKey(GLFW_KEY_1))
			SetActiveCam(GetCamWithIndex(1));
		if (GetKey(GLFW_KEY_SPACE))
			followIndex++;

		if (followIndex == GetObjectCount())
			followIndex = 0;

		if (GetCamWithIndex(1)->GetActive())
		{
			GetCamWithIndex(1)->Follow(GetObjectWithIndex(followIndex), 15.0f, 5.0f, followAngle);
			followAngle += 0.001f;
		}

		return true;
	}
private:
	GLuint followIndex;
	GLfloat followAngle;
};

int main()
{
	Test test;
	test.Run(1980, 1080);
}