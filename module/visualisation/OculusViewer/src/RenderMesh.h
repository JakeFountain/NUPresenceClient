
#include <GL/OOGL.hpp>

#ifndef MODULES_VISUALISATION_NUPRESENCE_RENDER_MESH_H
#define MODULES_VISUALISATION_NUPRESENCE_RENDER_MESH_H

namespace module {
namespace visualisation {

	class RenderMesh {
	public:
		RenderMesh::RenderMesh(const RenderMesh& m);
	    RenderMesh(std::string modelName, std::string textureName);
	    void render(GL::Context& gl, GL::Mat4 modelview, GL::Mat4 projection, GL::Program& shader);
		GL::Mesh mesh;
		GL::Image image;
		GL::Texture texture;
		GL::VertexArray vao;
		GL::VertexBuffer vBuffer;
	};

}
}
#endif
	