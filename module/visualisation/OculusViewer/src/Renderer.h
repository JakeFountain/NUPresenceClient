

#include "OVRManager.h"
#include "Scene.h"
#include <GL/OOGL.hpp>
#include <iostream>
#include <memory>

#ifndef NUPRESENCE_RENDERER
#define NUPRESENCE_RENDERER

class Renderer {
public:
    Renderer();
    void render();
private:
	std::unique_ptr<GL::Window> window;

	float width = 800;
	float height = 600;
	OVRManager ovrManager;

	std::unique_ptr<Scene> scene;

};

#endif
	