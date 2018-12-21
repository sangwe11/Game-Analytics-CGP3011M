#version 420 core

// In/out variables
in vec3 vertex;

void main()
{
	gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);	
}