

#include <GL/OOGL.hpp>
#include <iostream>

#ifndef NUPRESENCE_SCENE
#define NUPRESENCE_SCENE

class Scene {
public:
    Scene();

    void render(GL::Context& glContext, GL::Program& shaderProgram);
private:
	GL::VertexBuffer vbo;
	GL::VertexArray vao;
};

#endif