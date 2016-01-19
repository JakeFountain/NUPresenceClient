
#include <GL/glew.h>  
#include <oglplus/all.hpp>
#include <iostream>
#include "GameObject.h"

#ifndef NUPRESENCE_SCENE
#define NUPRESENCE_SCENE

class Scene {
public:
    Scene();

    void render(oglplus::Context& glContext, oglplus::Program& shaderProgram, oglplus::Mat4f view, oglplus::Mat4f projection);
private:
	GameObject rootObject;
};

#endif