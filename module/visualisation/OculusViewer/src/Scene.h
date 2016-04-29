
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

		void addScreen();

	private:
		GameObject rootObject;

		std::vector<std::shared_ptr<GameObject>> screens;
		int max_screens = 50;
		int current_screen = 0;
		int screen_count = 0;
		float FOVX = 1.0472;
    	float FOVY = 0.785389;
    	float screenDistance = 1;


		std::shared_ptr<GameObject> head_box;
		// std::shared_ptr<GameObject> screen;
		// std::shared_ptr<GameObject> screen_parent;
	};

}
}
#endif