

#include "OVRManager.h"
#include "Scene.h"
#include <GL/OOGL.hpp>
#include <iostream>
#include <memory>
#include <functional>

#include <GLFW/glfw3.h>

#ifndef NUPRESENCE_RENDERER
#define NUPRESENCE_RENDERER

class Renderer {
public:
    Renderer();
    void render(float t_sec);
private:
	//std::unique_ptr<GL::Window> window;
	std::unique_ptr<GLFWwindow,std::function<void(GLFWwindow*)>> window;
	std::unique_ptr<GL::Context> context;

	float width = 800;
	float height = 600;
	OVRManager ovrManager;

	std::unique_ptr<Scene> scene;
	std::unique_ptr<GL::Shader> vert;
	std::unique_ptr<GL::Shader> frag;
	std::unique_ptr<GL::Program> program;

};

#endif
	