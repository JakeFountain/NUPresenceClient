
#include "Scene.h"
#include <cmath>

namespace module {
namespace visualisation {

    Scene::Scene(){

		monkey = std::make_shared<GameObject>();
		screen_parent = std::make_shared<GameObject>();
		screen = std::make_shared<GameObject>(GL::Mat4().Translate(GL::Vec3(0, 0, -1)).RotateX(M_PI_2).RotateY(M_PI_2).Scale(GL::Vec3(1,1,-1)));
		auto static_screen = std::make_shared<GameObject>(GL::Mat4().Translate(GL::Vec3(0,0,-3)).RotateX(M_PI_2).RotateY(M_PI_2));
		try {
			monkey->addMesh(std::make_shared<RenderMesh>("../../../assets/monkey.obj", "../../../assets/monkey_texture2.png"));
			screen->addMesh(std::make_shared<RenderMesh>("../../../assets/screen.obj", "../../../assets/landscape.jpg"));
			static_screen->addMesh(std::make_shared<RenderMesh>("../../../assets/screen.obj", "../../../assets/landscape.jpg"));
		} catch (GL::FileException e) {
			std::cout << __FUNCTION__ << " Line: " << __LINE__ << "\nFile exception when loading meshes!" << std::endl;
		}
		screen_parent->addChild(screen);
		rootObject.addChild(screen_parent);
		rootObject.addChild(static_screen);
		rootObject.addChild(monkey);
    }

    void Scene::render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection, float t){
    	
		float camera_period = 10;
		float sin = std::sin(2 * 3.14 * t / camera_period);
		float cos = std::cos(2 * 3.14 * t / camera_period);
		monkey->transform = GL::Mat4::LookAt(GL::Vec3(0, 0, 0), GL::Vec3(sin, cos, 0), GL::Vec3(0, 0, 1));

    	glContext.UseProgram(shaderProgram);

	    rootObject.render(glContext, GL::Mat4(), view, projection, shaderProgram);

    }

    void Scene::setRobotImage(const WorldState::Image& image, const GL::Mat4& head_pose){

    	//TODO: change this to work with other image formats
		int rWidth = image.width * (screen->meshes[0]->texFormat == WorldState::Image::Format::YCBCR_422 ? 2 : 1);
		int rHeight = image.height;

		screen->meshes[0]->texture.Image2D(image.data.data(), GL::DataType::UnsignedByte, GL::Format::Red, rWidth, rHeight, GL::InternalFormat::Red);
		screen->meshes[0]->texFormat = image.format;
		screen->meshes[0]->texWidth = image.width;
		screen->meshes[0]->texHeight = image.height;

		screen_parent->transform = head_pose;

		screen->meshes[0]->texture.SetWrapping(GL::Wrapping::ClampEdge);
		screen->meshes[0]->texture.SetFilters(GL::Filter::Nearest, GL::Filter::Nearest);

    }

	void Scene::setRobotImagePose(const GL::Mat4& head_pose) {
		screen_parent->transform = head_pose;
	}

	GLuint Scene::getRobotEyeTexture() {
		return screen->meshes[0]->texture;
	}

}
}