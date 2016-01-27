
#include "Scene.h"


namespace module {
namespace visualisation {

    Scene::Scene():robotEyeTexture(){
		
		robotEyeTexture.SetWrapping(GL::Wrapping::ClampEdge);
		robotEyeTexture.SetFilters(GL::Filter::Nearest, GL::Filter::Nearest);

		GameObject monkey(GL::Mat4().Scale(GL::Vec3(1,1,1)));
		try {
			monkey.addMesh(RenderMesh("../../../assets/monkey.obj", "../../../assets/monkey_texture2.png"));
		} catch (GL::FileException e) {
			std::cout << "File exception! " << std::endl;
		}
		rootObject.addChild(monkey);
    }

    void Scene::render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection){
    	glContext.UseProgram(shaderProgram);

	    rootObject.render(glContext, GL::Mat4(), view, projection, shaderProgram);

    }

    void Scene::setRobotImage(const WorldState::Image& image){

    	//TODO: change this to work with other image formats
		int rWidth =  worldState.latestImage.width * 2;
		int rHeight = worldState.latestImage.height;

		robotEyeTexture.Image2D(image.data.data(), GL::DataType::UnsignedByte, GL::Format::Red, rWidth, rHeight, GL::InternalFormat::Red);

    }

}
}