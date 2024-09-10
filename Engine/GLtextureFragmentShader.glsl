#version 330 core

in vec2 v_TexCoord;

out vec4 color;


uniform sampler2D u_Texture;

void main () {

	//vec2 testCoord = vec2(0.35,0.4);
	//vec4 texColor = texture(u_Texture, testCoord);
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
	//color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}