#version 420 core

in vec3 colourIn;

out vec4 finalColor;

void main()
{	
	finalColor = vec4(colourIn, 0.2);
}