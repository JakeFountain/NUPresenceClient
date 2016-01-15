
// #include <GL/glew.h> 
// #include <oglplus/all.hpp>

// #ifndef TEXTURE_TO_SCREEN_H
// #define TEXTURE_TO_SCREEN_H

// class TextureToScreen{
// 	private:
// 		oglplus::Shader frag;
// 		oglplus::Shader vert;
// 		oglplus::Program shader;
// 		oglplus::VertexArray vao;
// 		oglplus::VertexBuffer vBuffer;

// 		float vertices[24] = {
// 			1, 1, 1, 1,
// 			1, -1, 1, 0,
// 			-1,-1, 0, 0,
// 			-1,-1, 0, 0,
// 			-1, 1, 0, 1,
// 			1, 1, 1, 1
// 		};


// 	public:
// 		TextureToScreen();
// 		void renderTextureToScreen(oglplus::Context& gl,oglplus::Texture tex);
// };

// #endif