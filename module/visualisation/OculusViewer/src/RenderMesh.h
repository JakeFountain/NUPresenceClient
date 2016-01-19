
#include <GL/glew.h>  
#include <oglplus/all.hpp>
#include <oglplus/shapes/blender_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#ifndef NUPRESENCE_RENDER_MESH_H
#define NUPRESENCE_RENDER_MESH_H

class RenderMesh {
public:

	RenderMesh::RenderMesh(const RenderMesh& m);
    RenderMesh(std::string modelName, std::string textureName);
    void render(oglplus::Context& gl, oglplus::Mat4f modelview, oglplus::Mat4f projection, oglplus::Program& shader);
private:
	oglplus::VertexArray mesh;
	oglplus::shapes::ObjMesh load_mesh;
	oglplus::shapes::DrawingInstructions mesh_instr;
	oglplus::shapes::ObjMesh::IndexArray mesh_indices;
	//oglplus::Spheref mesh_bs;

	oglplus::Buffer positions;
	oglplus::Buffer normals;
	oglplus::Buffer texcoords;
};

#endif
	