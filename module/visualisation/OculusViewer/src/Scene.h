
#include <GL/OOGL.hpp>
#include <iostream>
#include "GameObject.h"

#ifndef NUPRESENCE_SCENE
#define NUPRESENCE_SCENE

class Scene {
public:
    Scene();

    void render(GL::Context& glContext, GL::Program& shaderProgram);
private:
	GameObject rootObject;
};

#endif