

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
    void render(float t_sec);
private:
	std::unique_ptr<GL::Window> window;

	float width = 800;
	float height = 600;
	OVRManager ovrManager;

	std::unique_ptr<Scene> scene;
	std::unique_ptr<GL::Shader> vert;
	std::unique_ptr<GL::Shader> frag;
	std::unique_ptr<GL::Program> program;

};

#endif
	