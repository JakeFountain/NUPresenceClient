
#include "Scene.h"
#include <cmath>

namespace module {
namespace visualisation {

    Scene::Scene(){
    	
    	float head_box_size = 0.1;
		head_box = std::make_shared<GameObject>(GL::Mat4().Scale(GL::Vec3(head_box_size,head_box_size,head_box_size)));
		// auto static_screen = std::make_shared<GameObject>(GL::Mat4().Translate(GL::Vec3(0,0,-3)).RotateX(M_PI_2).RotateY(M_PI_2));
		// auto ground = std::make_shared<GameObject>(GL::Mat4().Translate(GL::Vec3(0,-1.5,0)).Scale(GL::Vec3(100,100,100)));
		// auto skybox = std::make_shared<GameObject>(GL::Mat4().RotateX(-M_PI_2).Scale(GL::Vec3(100,100,100)));
		try {
			head_box->addMesh(std::make_shared<RenderMesh>("../../../assets/unit_cube.obj", "../../../assets/cube_edges.png"));
			// static_screen->addMesh(std::make_shared<RenderMesh>("../../../assets/screen.obj", "../../../assets/landscape.jpg"));
			// ground->addMesh(std::make_shared<RenderMesh>("../../../assets/ground.obj", "../../../assets/panels.jpg"));
			// skybox->addMesh(std::make_shared<RenderMesh>("../../../assets/unit_cube.obj", "../../../assets/skycube.jpg"));
		} catch (GL::FileException e) {
			std::cout << __FUNCTION__ << " Line: " << __LINE__ << "\nFile exception when loading meshes!" << std::endl;
		}

		//Setup screens
		float scaleX = screenDistance * std::tan(FOVX / 2);
    	float scaleY = screenDistance * std::tan(FOVY / 2);
    	
    	for(int i = 0; i < max_screens; i++){
			auto screen = std::make_shared<GameObject>(GL::Mat4().Translate(GL::Vec3(0, 0, -screenDistance)).RotateX(M_PI_2).RotateY(M_PI_2).Scale(GL::Vec3(scaleY,1,-scaleX)));
			auto screen_parent = std::make_shared<GameObject>();
			try {
				screen->addMesh(std::make_shared<RenderMesh>("../../../assets/screen.obj"));			
			} catch (GL::FileException e) {
				std::cout << __FUNCTION__ << " Line: " << __LINE__ << "\nFile exception when loading meshes!" << std::endl;
			}
			screen_parent->addChild(screen);
			screens.push_back(screen_parent);
		}

		//rootObject.addChild(static_screen);
		//rootObject.addChild(skybox);
		//rootObject.addChild(ground);
		// rootObject.addChild(head_box);
    }

    void Scene::render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection, float t){
    	
		float camera_period = 10;
		float sin = std::sin(2 * 3.14 * t / camera_period);
		float cos = std::cos(2 * 3.14 * t / camera_period);
		//head_box->transform = GL::Mat4::LookAt(GL::Vec3(0, 0, 0), GL::Vec3(sin, cos, 0), GL::Vec3(0, 0, 1));

    	glContext.UseProgram(shaderProgram);
	    rootObject.render(glContext, GL::Mat4(), view, projection, shaderProgram);


		glContext.Disable(GL::Capability::DepthTest);
	    int screens_rendered = 0;
	    int i = current_screen;
		while (screens_rendered < screen_count) {
			screens[i]->render(glContext, GL::Mat4(), view, projection, shaderProgram);
			i = (i + 1) % max_screens;
			screens_rendered++;
		}
		glContext.Enable(GL::Capability::DepthTest);

    }

    void Scene::setRobotImage(const WorldState::Image& image, const GL::Mat4& head_pose){
    	current_screen = (current_screen + 1) % max_screens;
    	//TODO: change this to work with other image formats
		int rWidth = image.width * (screens.back()->children[0]->meshes[0]->texFormat == WorldState::Image::Format::YCBCR_422 ? 2 : 1);
		int rHeight = image.height;

		screens[current_screen]->children[0]->meshes[0]->texture.Image2D(image.data.data(), GL::DataType::UnsignedByte, GL::Format::Red, rWidth, rHeight, GL::InternalFormat::Red);
		screens[current_screen]->children[0]->meshes[0]->texFormat = image.format;
		screens[current_screen]->children[0]->meshes[0]->texWidth = image.width;
		screens[current_screen]->children[0]->meshes[0]->texHeight = image.height;

		screens[current_screen]->transform = head_pose;

		screens[current_screen]->children[0]->meshes[0]->texture.SetWrapping(GL::Wrapping::ClampEdge);
		screens[current_screen]->children[0]->meshes[0]->texture.SetFilters(GL::Filter::Nearest, GL::Filter::Nearest);

		screen_count = max(screen_count, max_screens);

    }

	void Scene::setRobotImagePose(const GL::Mat4& head_pose) {
		screens[current_screen]->transform = head_pose;
	}

	GLuint Scene::getRobotEyeTexture() {
		return screens[current_screen]->children[0]->meshes[0]->texture;
	}

}
}