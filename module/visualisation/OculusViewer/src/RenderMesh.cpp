
#include "RenderMesh.h"

RenderMesh::RenderMesh(const RenderMesh& m) : meshes(m.meshes) {
	
}


RenderMesh::RenderMesh(std::string modelName, std::string textureName):
load_mesh(mesh_input.stream), 
mesh_instr(load_mesh.Instructions()), 
mesh_indices(load_mesh.Indices())
{
	mesh.Bind();

	positions.Bind(Buffer::Target::Array);
	{
		std::vector<GLfloat> data;
		GLuint n_per_vertex = load_mesh.Positions(data);
		Buffer::Data(Buffer::Target::Array, data);
	}

	normals.Bind(Buffer::Target::Array);
	{
		std::vector<GLfloat> data;
		GLuint n_per_vertex = load_mesh.Normals(data);
		Buffer::Data(Buffer::Target::Array, data);
	}

}


void RenderMesh::render(oglplus::Context& gl, oglplus::Mat4f modelview, oglplus::Mat4f projection, oglplus::Program& shader) {
	shader.Use();

	gl.Enable(Capability::DepthTest);
	(shader|"Position").Setup<GLfloat>().Enable();
	(shader|"Normal").Setup<GLfloat>(3).Enable();
	(shader|"Texcoord").Setup<GLfloat>(2).Enable();
	
	shader.

}
