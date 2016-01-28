#version 150


in vec3 pos;
in vec3 normal;
in vec2 texcoord;
out vec4 Pos;
out vec4 Normal;
out vec2 Texcoord;
uniform mat4 modelview;
uniform mat4 projection;
void main() {
	Pos = modelview * vec4(pos,1.0);
	Normal = modelview * vec4(normal,0.0);
	Texcoord = texcoord;
	gl_Position = projection * vec4(Pos.x,-Pos.y, Pos.z, 1.0);
}
