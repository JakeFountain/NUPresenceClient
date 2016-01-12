
#include <GL/OOGL.hpp>
#include <iostream>
#include "GameObject.h"

#ifndef NUPRESENCE_SCENE
#define NUPRESENCE_SCENE

class Scene {
public:
    Scene();

    void render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection);
private:
	GameObject rootObject;
};

#endif