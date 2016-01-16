#define OGLPLUS_NO_SITE_CONFIG

#include "OVRManager.h"
#include "Scene.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <oglplus/all.hpp>
#include <iostream>
#include <memory>
#include <functional>

#ifndef NUPRESENCE_RENDERER
#define NUPRESENCE_RENDERER

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();
    bool render(float t_sec);
private:
	std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> window;

	float width = 800;
	float height = 600;
//	OVRManager ovrManager;

	
	struct Graphics{
		oglplus::Context context;
		oglplus::Program program;
		Scene scene;
	};

	std::unique_ptr<Graphics> gl;

};

#endif
	