
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

		float vertices[24] = {
			1, 1, 1, 1,
			1, -1, 1, 0,
			-1,-1, 0, 0,
			-1,-1, 0, 0,
			-1, 1, 0, 1,
			1, 1, 1, 1
		};


	public:
		TextureToScreen();
		void renderTextureToScreen(GL::Context& gl,GL::Texture tex);
};

#endif