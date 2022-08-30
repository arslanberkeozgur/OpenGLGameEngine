#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetColorLocation()
{
	return uniformColor;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);
	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexCode = ReadFile(vertexLocation);
	std::string fragmentCode = ReadFile(fragmentLocation);
	CompileShader(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	// 1. Create empty program.
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program\n");
		exit(1);
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	// 6. Link program. (create executables from shaders and link them together)
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	// 7. Validate program. (check if program is valid)
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	// Return IDs of uniform variables.
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformColor = glGetUniformLocation(shaderID, "vColor");
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	// 2. Create empty shader.
	GLuint theShader = glCreateShader(shaderType);

	// 3. Attach shader source code to shader.
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);

	// 4. Compile the shader.
	glCompileShader(theShader);

	// Check the compile status.
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(theShader, sizeof(errorLog), NULL, errorLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, errorLog);
		exit(1);
	}

	// 5. Attach the compiled shader to the program.
	glAttachShader(theProgram, theShader);

	return;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::DropShader()
{
	glUseProgram(0);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	uniformColor = 0;
}

Shader::~Shader()
{
	ClearShader();
}