
#include "RenderMesh.h"
RenderMesh::RenderMesh(const RenderMesh& m) : load_mesh(m.load_mesh), mesh_instr(m.mesh_instr), mesh_indices(m.mesh_indices) {
	
}


RenderMesh::RenderMesh(std::string modelName, std::string textureName):
	load_mesh(std::ifstream(modelName)),
	mesh_instr(load_mesh.Instructions()),
	mesh_indices(load_mesh.Indices())
{
	mesh.Bind();

	positions.Bind(oglplus::Buffer::Target::Array);
	std::vector<GLfloat> data;
	GLuint n_per_vertex = load_mesh.Positions(data);
	oglplus::Buffer::Data(oglplus::Buffer::Target::Array, data);
	

	normals.Bind(oglplus::Buffer::Target::Array);
	data.clear();
	n_per_vertex = load_mesh.Normals(data);
	std::cout << "n_per_vertex = " << n_per_vertex << std::endl;
	oglplus::Buffer::Data(oglplus::Buffer::Target::Array, data);


	texcoords.Bind(oglplus::Buffer::Target::Array);
	data.clear();
	n_per_vertex = load_mesh.TexCoordinates(data);
	oglplus::Buffer::Data(oglplus::Buffer::Target::Array, data);
}


void RenderMesh::render(oglplus::Context& gl, oglplus::Mat4f modelview, oglplus::Mat4f projection, oglplus::Program& shader) {
	mesh.Bind();
	shader.Use();
	//shader.
	//oglplus::Error err;
	
	gl.Enable(oglplus::Capability::DepthTest);

	positions.Bind(oglplus::Buffer::Target::Array);
	(shader|"pos").Setup<oglplus::Vec3f>().Enable();

	texcoords.Bind(oglplus::Buffer::Target::Array);
	(shader|"texcoord").Setup<oglplus::Vec2f>().Enable();

	//normals.Bind(oglplus::Buffer::Target::Array);
	//auto normals = (shader | "normals");
	//normals.Setup<oglplus::Vec3f>();
	//normals.Enable();

	mesh_instr.Draw(mesh_indices);


}
