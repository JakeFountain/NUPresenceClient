
#include <GL/glew.h>  
#include <oglplus/all.hpp>
#include <oglplus/shapes/blender_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#ifndef NUPRESENCE_RENDER_MESH_H
#define NUPRESENCE_RENDER_MESH_H

using namespace oglplus;
class RenderMesh {
public:
	struct MeshInputFile
	{
		std::ifstream stream;

		MeshInputFile(std::string filename)
		{
			oglplus::OpenResourceFile(stream, "models", filename, ".obj");
		}
	};

	RenderMesh::RenderMesh(const RenderMesh& m);
    RenderMesh(std::string modelName, std::string textureName);
    void render(oglplus::Context& gl, oglplus::Mat4f modelview, oglplus::Mat4f projection, oglplus::Program& shader);
private:
	MeshInputFile mesh_input;
	shapes::ObjMesh load_mesh;
	shapes::DrawingInstructions mesh_instr;
	shapes::ObjMesh::IndexArray mesh_indices;

	VertexArray mesh;
	Spheref mesh_bs;

	Buffer positions;
	Buffer normals;
	Buffer texcoords;
};

#endif
	