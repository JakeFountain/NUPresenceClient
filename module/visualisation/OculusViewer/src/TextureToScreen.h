
#include <GL\OOGL.hpp>

#ifndef TEXTURE_TO_SCREEN_H
#define TEXTURE_TO_SCREEN_H

class TextureToScreen{
	private:
		GL::Shader frag;
		GL::Shader vert;
		GL::Program shader;
		GL::VertexArray vao;
		GL::VertexBuffer vBuffer;

	public:
		TextureToScreen();
		void renderTextureToScreen(GL::Context & gl, GLuint tex);
};

#endif