
#include "TextureToScreen.h"
#include <iostream>
#include <fstream>
#include "utility/file/fileutil.h"

namespace module {
namespace visualisation {

	TextureToScreen::TextureToScreen():
		vert(GL::ShaderType::Vertex, utility::file::loadFromFile("../../shaders/texToScreen.vert")),
		frag(GL::ShaderType::Fragment, utility::file::loadFromFile("../../shaders/texToScreen.frag")),
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

	void TextureToScreen::renderTextureToScreen(GL::Context& gl, GLuint tex, const WorldState::Image::Format& format, int width, int height){

		vao.BindAttribute(shader.GetAttribute( "pos" ), vBuffer, GL::Type::Float, 2, sizeof(float) * 4, 0 );
		vao.BindAttribute(shader.GetAttribute( "texcoord" ), vBuffer, GL::Type::Float, 2, sizeof(float) * 4, sizeof(float) * 2 );
		
		gl.UseProgram(shader);

		shader.SetUniform("format", int(format));
		shader.SetUniform("imageWidth", int(width));
		shader.SetUniform("imageHeight", int(height));

		shader.SetUniform("tex", 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, tex);

		gl.DrawArrays( vao, GL::Primitive::Triangles, 0, 6 );
	}

}
}