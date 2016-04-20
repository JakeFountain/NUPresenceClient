

#include "OVRManager.h"
#include "WorldState.h"
#include "Scene.h"
#include <GL/OOGL.hpp>
#include <iostream>
#include <memory>
#include <functional>

#include <GLFW/glfw3.h>

#ifndef MODULES_VISUALISATION_NUPRESENCE_RENDERER
#define MODULES_VISUALISATION_NUPRESENCE_RENDERER


namespace module {
namespace visualisation {

	class Renderer {
	public:
	    Renderer();
		void recenter();
	    bool render(float t_sec, const std::shared_ptr<const WorldState> worldState, GL::Mat4& userState);
	    void setScreenAngleThreshold(float t){cosCamAngleThreshold = t;}
	private:
		//std::unique_ptr<GL::Window> window;
		std::unique_ptr<GLFWwindow,std::function<void(GLFWwindow*)>> window;

		float width = 800;
		float height = 600;
		OVRManager ovrManager;

		std::unique_ptr<TextureToScreen> texToScreenRenderer;


		std::unique_ptr<Scene> scene;
		std::unique_ptr<GL::Shader> vert;
		std::unique_ptr<GL::Shader> frag;
		std::unique_ptr<GL::Program> program;
		
		GL::Vec3 last_look;
		float cosCamAngleThreshold = 0.85;
	};

}
}
#endif
		