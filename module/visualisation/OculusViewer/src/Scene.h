
#include <GL/OOGL.hpp>
#include <iostream>
#include "GameObject.h"
#include "WorldState.h"

#ifndef MODULES_VISUALISATION_NUPRESENCE_SCENE
#define MODULES_VISUALISATION_NUPRESENCE_SCENE


namespace module {
namespace visualisation {


	class Scene {
	public:
	    Scene();

	    void render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection, float t);
    	void setRobotImage(const WorldState::Image& image, const GL::Mat4& head_pose);

		void setRobotImagePose(const GL::Mat4 & head_pose);

		GLuint getRobotEyeTexture();

	private:
		GameObject rootObject;

		std::shared_ptr<GameObject> monkey;
		std::shared_ptr<GameObject> screen;
		std::shared_ptr<GameObject> screen_parent;
	};

}
}
#endif