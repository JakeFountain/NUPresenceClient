

#include "OVRManager.h"
#include "Scene.h"
#include <GL/OOGL.hpp>
#include <iostream>

#ifndef NUPRESENCE_RENDERER
#define NUPRESENCE_RENDERER

class Renderer {
public:
    Renderer();
    void render();
private:
	float width = 800;
	float height = 600;
	OVRManager ovrManager;
	GL::Window window;

	Scene scene;

};

#endif