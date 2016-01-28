
#include "Scene.h"


namespace module {
namespace visualisation {

    Scene::Scene(){

		monkey = std::make_shared<GameObject>(GL::Mat4().Scale(GL::Vec3(1,1,1)));
		screen = std::make_shared<GameObject>();
		try {
			monkey->addMesh(std::make_shared<RenderMesh>("../../../assets/monkey.obj", "../../assets/monkey_texture2.png"));
			screen->addMesh(std::make_shared<RenderMesh>("../../../assets/screen.obj", "../../assets/monkey_texture2.png"));
		} catch (GL::FileException e) {
			std::cout << "File exception! " << std::endl;
		}

		rootObject.addChild(screen);
		rootObject.addChild(monkey);
    }

    void Scene::render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection){
    	glContext.UseProgram(shaderProgram);

	    rootObject.render(glContext, GL::Mat4(), view, projection, shaderProgram);

    }

    void Scene::setRobotImage(const WorldState::Image& image){

    	//TODO: change this to work with other image formats
		int rWidth = image.width * 2;
		int rHeight = image.height;

		screen->meshes[0]->texture.Image2D(image.data.data(), GL::DataType::UnsignedByte, GL::Format::Red, rWidth, rHeight, GL::InternalFormat::Red);

    }

	GLuint Scene::getRobotEyeTexture() {
		return screen->meshes[0]->texture;
	}

}
}