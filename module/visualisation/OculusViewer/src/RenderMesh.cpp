
#include "RenderMesh.h"


namespace module {
namespace visualisation {

	RenderMesh::RenderMesh(const RenderMesh& m) : mesh(m.mesh), texture(m.texture), image() {
		vao = m.vao;
		vBuffer = m.vBuffer;
	}


	RenderMesh::RenderMesh(std::string modelName, std::string textureName) : 
		vao(),
		image(),
		texture(),
		mesh(modelName),
		vBuffer( mesh, GL::BufferUsage::StaticDraw, [] ( const GL::Vertex& v, GL::VertexDataBuffer& data )
		{
			data.Vec3( v.Pos );
			data.Vec3( v.Normal );
			data.Vec2( v.Tex );
		} )
	{
		image.Load(textureName);
		texture = GL::Texture(image);
	}


	void RenderMesh::render(GL::Context& gl, GL::Mat4 modelview, GL::Mat4 projection, GL::Program& shader) {
		glEnable(GL_DEPTH_TEST);
		vao.BindAttribute(shader.GetAttribute( "pos" ), vBuffer, GL::Type::Float, 3, sizeof(float) * 8, 0 );
		//vao.BindAttribute(shader.GetAttribute( "normal" ), vBuffer, GL::Type::Float, 3, sizeof(float) * 8, sizeof(float) * 3 );
		vao.BindAttribute(shader.GetAttribute( "texcoord" ), vBuffer, GL::Type::Float, 2, sizeof(float) * 8, sizeof(float) * 6 );
		
		shader.SetUniform("modelview", modelview);
		shader.SetUniform("projection", projection);
		
		gl.BindTexture(texture, 0);
		shader.SetUniform("tex", 0);

		gl.UseProgram(shader);
		gl.DrawArrays( vao, GL::Primitive::Triangles, 0, mesh.VertexCount() );

	}

}
}