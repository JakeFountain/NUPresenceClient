
#include <GL/OOGL.hpp>
#include <iostream>
#include "GameObject.h"

#ifndef MODULES_VISUALISATION_NUPRESENCE_SCENE
#define MODULES_VISUALISATION_NUPRESENCE_SCENE


namespace module {
namespace visualisation {


	class Scene {
	public:
	    Scene();

	    void render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection);
	private:
		GameObject rootObject;
	};

}
}
#endif