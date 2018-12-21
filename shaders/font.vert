
#version 330 core
 
in vec4 vertex;
out vec2 uv;
 
void main(void) {

	uv = vertex.zw;
	gl_Position = vec4(vertex.xy, 0, 1);
  
}