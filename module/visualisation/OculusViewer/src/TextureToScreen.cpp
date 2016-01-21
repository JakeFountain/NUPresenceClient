
#include "TextureToScreen.h"
#include <iostream>

TextureToScreen::TextureToScreen():
	vert(GL::ShaderType::Vertex, GLSL(
				in vec2 pos;
				in vec2 texcoord;
				out vec2 Texcoord;
				void main() {
					Texcoord = texcoord;
					gl_Position = vec4(pos,0,1);
				}
			)),
	frag(GL::ShaderType::Fragment, GLSL(
				in vec2 Texcoord;
				out vec4 outColor;
				uniform sampler2D tex;
				void main() {
					outColor = texture(tex, vec2(Texcoord.x, Texcoord.y));
				}
			)),
	shader(frag, vert),
	vao()
	
{
	float vertices[24] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f, 0.0f,
		-1.0f,-1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	vBuffer = GL::VertexBuffer(vertices, 24 * sizeof(float), GL::BufferUsage::StaticDraw);

}

void TextureToScreen::renderTextureToScreen(GL::Context& gl, GL::Texture tex){

	vao.BindAttribute(shader.GetAttribute( "pos" ), vBuffer, GL::Type::Float, 2, sizeof(float) * 4, 0 );
	vao.BindAttribute(shader.GetAttribute( "texcoord" ), vBuffer, GL::Type::Float, 2, sizeof(float) * 4, sizeof(float) * 2 );
	
	gl.BindTexture(tex, 0);
	shader.SetUniform("tex", 0);
	
	gl.UseProgram(shader);
	gl.DrawArrays( vao, GL::Primitive::Triangles, 0, 6 );
}
