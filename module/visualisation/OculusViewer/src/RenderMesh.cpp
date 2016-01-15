
// #include "RenderMesh.h"

// RenderMesh::RenderMesh(const RenderMesh& m) : mesh(m.mesh), texture(m.texture), image() {
// 	vao = m.vao;
// 	vBuffer = m.vBuffer;
// }


// RenderMesh::RenderMesh(std::string modelName, std::string textureName) : 
// 	{
// 		oglplus::image::Load(textureName);
// 		texture = oglplus::Texture(image);
// 	}


// void RenderMesh::render(oglplus::Context& gl, oglplus::Mat4 modelview, oglplus::Mat4 projection, oglplus::Program& shader) {
// 	glEnable(GL_DEPTH_TEST);
// 	vao.BindAttribute(shader.GetAttribute( "pos" ), vBuffer, oglplus::Type::Float, 3, sizeof(float) * 8, 0 );
// 	//vao.BindAttribute(shader.GetAttribute( "normal" ), vBuffer, oglplus::Type::Float, 3, sizeof(float) * 8, sizeof(float) * 3 );
// 	vao.BindAttribute(shader.GetAttribute( "texcoord" ), vBuffer, oglplus::Type::Float, 2, sizeof(float) * 8, sizeof(float) * 6 );
// 	shader.SetUniform("modelview", modelview);
// 	shader.SetUniform("projection", projection);
// 	gl.BindTexture(texture, 0);
// 	shader.SetUniform("tex", 0);

// 	gl.UseProgram(shader);
// 	gl.DrawArrays( vao, oglplus::Primitive::Triangles, 0, mesh.VertexCount() );

// }
