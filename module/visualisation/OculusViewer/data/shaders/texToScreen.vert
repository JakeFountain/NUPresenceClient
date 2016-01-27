#version 150

in vec2 pos;
in vec2 texcoord;
out vec2 Texcoord;
void main() {
	Texcoord = texcoord;
	gl_Position = vec4(pos,0,1);
}