

#include "Renderer.h"
#include "utility/file/fileutil.h"
#include "utility/math/Conversion.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>



namespace module {
namespace visualisation {

	Renderer::Renderer():
		ovrManager()
	{
	}

	    
	bool Renderer::render(float t_sec, const std::shared_ptr<const WorldState> worldState, GL::Mat4& userState){
		
		if(!window){
			width = 1920 ;
			height = 1080;

			glfwSetErrorCallback([](int, const char* msg) {
				std::cout << msg << std::endl;
			});

			glfwInit();

			window = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(glfwCreateWindow(width, height, "Visualisation Window", nullptr, nullptr),glfwDestroyWindow);
			if (!window) {
				std::cout << "GLFW window creation failed: " << std::endl;
			}

			glfwMakeContextCurrent(window.get());

			GL::Context context = GL::Context::UseExistingContext();

			scene = std::make_unique<Scene>();

			try {
				texToScreenRenderer = std::make_unique<TextureToScreen>();

				vert = std::make_unique<GL::Shader>(GL::ShaderType::Vertex, utility::file::loadFromFile("../../shaders/meshShader.vert"));

				frag = std::make_unique<GL::Shader>(GL::ShaderType::Fragment, utility::file::loadFromFile("../../shaders/meshShader.frag"));

				program = std::make_unique<GL::Program>(*vert, *frag);

				bool ovrInitResult = ovrManager.init();
				if (!ovrInitResult) {
					std::cout << "OVR Failed to initialise!" << std::endl;
				}

			} catch (GL::CompileException e) {
				std::cout << e.what() << std::endl;
				throw e;
			}


		}

		auto origin = GL::Mat4();// ::LookAt(GL::Vec3(0, 0, 0), GL::Vec3(1, 0, 0), GL::Vec3(0, 0, 1));
		GL::Mat4 rawHeadPose = ovrManager.getRawHeadPose() * origin;

		if(worldState){
			scene->setRobotImage(worldState->latestImage, rawHeadPose.Inverse());
		}
		else {
			scene->setRobotImagePose(rawHeadPose.Inverse());
		}

		GL::Context context = GL::Context::UseExistingContext();
		if (!glfwWindowShouldClose(window.get()))
	    {
			auto poses = ovrManager.getCurrentPoses();
			
			//Draw eye buffers
			int eyeNumber = 0;
			for (auto& pose : poses) {
				GL::Mat4 view = pose.view * origin;
				ovrManager.setRenderTarget(context, OVRManager::RenderTarget(eyeNumber));
				
				if (eyeNumber == 0) {
					//glClearColor(1, 0, 0, 1);
					context.Clear();
					glClearColor(0, 0, 0, 1);

				}
				scene->render(context, *program, view, pose.proj, t_sec);
				eyeNumber++;
			}

			
			if(!poses.empty()){
				//Draw to rift
				bool renderResult = ovrManager.renderToRift();
				if (!renderResult) {
					std::cout << "Rift display failed!" << std::endl;
				}
			}

			//Draw to mirror
			context.Disable(GL::Capability::DepthTest);
			context.BindFramebuffer(); //Bind to screen
			context.Clear();

			GL::Mat4 proj = GL::Mat4::Perspective(1.0, width / float(height), 0.01, 100);
			proj = proj.Scale(GL::Vec3(1, -1, 1));

			if (!poses.empty()) {
				//TODO:fix mirroring
				//GLuint eyeTex = ovrManager.getLastEyeTexture();
				//texToScreenRenderer->renderTextureToScreen(context, eyeTex);

				//Test robot image
				//if(worldState){
				//	texToScreenRenderer->renderTextureToScreen(context, scene->getRobotEyeTexture(), worldState->latestImage.format, worldState->latestImage.width, worldState->latestImage.height);
				//}
				//For now re-render
				scene->render(context, *program, rawHeadPose, proj, t_sec);
			}
			else {
				if(worldState){
					texToScreenRenderer->renderTextureToScreen(context, scene->getRobotEyeTexture(), worldState->latestImage.format, worldState->latestImage.width, worldState->latestImage.height);
				}
				//GL::Mat4 view = GL::Mat4::LookAt(GL::Vec3(1, 1, 1), GL::Vec3(0, 0, 0), GL::Vec3(0, 1, 0)) * origin;
				//scene->render(context, *program, view, proj, t_sec);
			}


			glfwSwapBuffers(window.get());

			glfwPollEvents();

			if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) > 0) {
				glfwSetWindowShouldClose(window.get(), 1);
			}
			return true;
		}
		return false;

		
	}


}
}