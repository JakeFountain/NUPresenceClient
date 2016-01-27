
#include <GL\OOGL.hpp>

#ifndef MODULES_VISUALISATION_TEXTURE_TO_SCREEN_H
#define MODULES_VISUALISATION_TEXTURE_TO_SCREEN_H


namespace module {
namespace visualisation {

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

}
}

#endif