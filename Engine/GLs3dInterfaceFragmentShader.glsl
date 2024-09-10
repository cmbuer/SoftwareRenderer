#version 330 core



void main () {
	gl_Position = vec4(2*vpos.xy / viewport.xy - 1, 0, 1);
}
