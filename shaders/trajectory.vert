#version 420 core

// In/out variables
in vec3 vertex;
in vec3 colour;

out vec3 colourIn;

uniform mat4 mvp;
uniform bool speed;
uniform vec3 lineColour;

void main()
{
	if(speed)
		colourIn = colour;
	else
	colourIn = lineColour;
	
	gl_Position = mvp * vec4(vertex.x, vertex.y, 1.0, 1.0);
	gl_PointSize = 5.0;
}