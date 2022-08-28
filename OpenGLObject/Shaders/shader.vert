#version 330 core
layout (location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 vColor;
void main()
{		
	gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
	vColor = vec4(pos, 1.0f);
}					