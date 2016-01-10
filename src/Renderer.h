

#include "OVRManager.h"
#include "Scene.h"
#include <GL/OOGL.hpp>
#include <iostream>

#ifndef NUPRESENCE_RENDERER
#define NUPRESENCE_RENDERER

class Renderer {
public:
    Renderer();

    void run();
private:
	float width = 800;
	float height = 600;
	OVRManager ovrManager;
	GL::Window window;
};

#endif