
// #include "TextureToScreen.h"
// #include <iostream>

// TextureToScreen::TextureToScreen():
// 	vert(oglplus::ShaderType::Vertex, GLSL(
// 				in vec2 pos;
// 				in vec2 texcoord;
// 				out vec2 Texcoord;
// 				void main() {
// 					Texcoord = texcoord;
// 					gl_Position = vec4(pos,0,1);
// 				}
// 			)),
// 	frag(oglplus::ShaderType::Fragment, GLSL(
// 				in vec2 Texcoord;
// 				out vec4 outColor;
// 				uniform sampler2D tex;
// 				void main() {
// 					outColor = vec4(1,0,0,1);// texture(tex, vec2(Texcoord.x, Texcoord.y));
// 				}
// 			)),
// 	shader(frag, vert),
// 	vao(),
// 	vBuffer(vertices, 24 * sizeof(float), oglplus::BufferUsage::StaticDraw)
// {

// }

// void TextureToScreen::renderTextureToScreen(oglplus::Context& gl, oglplus::Texture tex){
// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;
// 	gl.BindFramebuffer();
// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;

// 	gl.UseProgram(shader);

// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;
// 	vao.BindAttribute(shader.GetAttribute( "pos" ), vBuffer, oglplus::Type::Float, 2, sizeof(float) * 4, 0 );
// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;
// 	vao.BindAttribute(shader.GetAttribute( "texcoord" ), vBuffer, oglplus::Type::Float, 2, sizeof(float) * 4, sizeof(float) * 2 );
	
// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;
// 	gl.BindTexture(tex, 0);
// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;
// 	shader.SetUniform("tex", 0);
	
// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;
// 	gl.DrawArrays( vao, oglplus::Primitive::Triangles, 0, 6 );
// 	std::cout << __FILE__ << ":  " << __LINE__ << std::endl;
// }
