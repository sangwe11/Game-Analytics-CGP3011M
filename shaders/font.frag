#version 330 core
 
in vec2 uv;
uniform sampler2D font;
uniform vec4 fontColor;
 
void main(void) {

	float alpha = dot(vec3(0.2989, 0.5870, 0.1140), texture(font, uv).rgb);
	
	if(alpha == 0)
		discard;

	gl_FragColor = vec4(1, 1, 1, 1);
  
}