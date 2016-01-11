
#include <GL/OOGL.hpp>

#ifndef NUPRESENCE_RENDER_MESH_H
#define NUPRESENCE_RENDER_MESH_H

class RenderMesh {
public:
	RenderMesh::RenderMesh(const RenderMesh& m);
    RenderMesh(std::string modelName, std::string textureName);
    void render(GL::Context& gl, GL::Mat4 modelview, GL::Mat4 projection, GL::Program& shader);
private:
	GL::Mesh mesh;
	GL::Texture texture;
	GL::Image image;
	GL::VertexArray vao;
	GL::VertexBuffer vBuffer;

};

#endif
	