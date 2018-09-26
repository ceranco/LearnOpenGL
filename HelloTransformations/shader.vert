#version 330 core

layout (location = 0) in vec3 aPosition;
//layout (location = 0) vec4 aColor;

out vec4 color;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPosition, 1.0);
	color = vec4(1.0, 0.2, 0.3, 1.0);
}