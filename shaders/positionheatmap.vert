#version 420 core

// In/out variables
in vec3 vertex;
in vec3 colour;

out vec3 colourIn;

uniform mat4 mvp;
uniform float size;

void main()
{
	colourIn = colour;
	gl_Position = mvp * vec4(vertex.x, vertex.y, 1.0, 1.0);
	gl_PointSize = size;
}