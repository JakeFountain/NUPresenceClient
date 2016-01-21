

#include "Renderer.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

Renderer::Renderer():
	ovrManager()
{
}

    
bool Renderer::render(float t_sec){
	
	if(!window){
		width = 1920 / 2;
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

		spareBuffer = std::make_unique<GL::Framebuffer>(width,height);


		try {
			texToScreenRenderer = std::make_unique<TextureToScreen>();

			vert = std::make_unique<GL::Shader>(GL::ShaderType::Vertex, GLSL(
				in vec3 pos;
				in vec3 normal;
				in vec2 texcoord;
				out vec4 Pos;
				out vec4 Normal;
				out vec2 Texcoord;
				uniform mat4 modelview;
				uniform mat4 projection;
				void main() {
					Pos = modelview * vec4(pos,1.0);
					Normal = modelview * vec4(normal,0.0);
					Texcoord = texcoord;
					gl_Position = projection * vec4(Pos.x,-Pos.y, Pos.z, 1.0);
				}
			));

			frag = std::make_unique<GL::Shader>(GL::ShaderType::Fragment, GLSL(
				in vec4 Pos;
				in vec4 Normal;
				in vec2 Texcoord;
				out vec4 outColor;
				uniform sampler2D tex;
				void main() {
					outColor = texture(tex, vec2(Texcoord.x, 1 - Texcoord.y));
					//outColor = vec4(mod(abs(Pos.z),1), 0, 0, 1);
				}
			));

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


	GL::Context context = GL::Context::UseExistingContext();
	if (!glfwWindowShouldClose(window.get()))
    {
		context.Clear();
		
		float camera_period = 10;
		float sin = std::sin(2 * 3.14 * t_sec / camera_period);
		float cos = std::cos(2 * 3.14 * t_sec / camera_period);
		GL::Mat4 origin = GL::Mat4::LookAt(GL::Vec3(0, 0, 0), GL::Vec3(sin, cos, 0), GL::Vec3(0, 0, 1));

		auto poses = ovrManager.getCurrentPoses();
		
		//Draw eye buffers
		int eyeNumber = 0;
		for (auto& pose : poses) {
			GL::Mat4 view = pose.view * origin;
			ovrManager.setRenderTarget(context, OVRManager::RenderTarget(eyeNumber));
			if(eyeNumber == 0) context.Clear();
			scene->render(context, *program, view, pose.proj);
			eyeNumber++;
		}
		
		if(poses.size() > 0){
			//Draw to rift
			bool renderResult = ovrManager.renderToRift();	
			if (!renderResult) {
				std::cout << "Rift display failed!" << std::endl;
			}
		} else {
			context.BindFramebuffer(*spareBuffer);
			context.Clear();
			GL::Mat4 view = GL::Mat4::LookAt(GL::Vec3(1, 1, 1), GL::Vec3(0, 0, 0), GL::Vec3(0, 0, 1)) * origin;
			GL::Mat4 proj = GL::Mat4::Perspective(1.0, width / float(height), 0.1, 100);
			scene->render(context, *program, view, proj);
		}

		//Draw to mirror
		context.Disable(GL::Capability::DepthTest);
		context.BindFramebuffer(); //Bind to screen
		context.Clear();
		//GL::Mat4 view = poses[0].view * origin;
		//GL::Mat4 proj = poses[0].proj.Scale(GL::Vec3(1, -1, 1));
		//scene->render(context, *program, view, proj);
		if (poses.size() > 0) {
			texToScreenRenderer->renderTextureToScreen(context, GL::Texture(ovrManager.getCurrentEyeBuffer()));
		}
		else {
			texToScreenRenderer->renderTextureToScreen(context, spareBuffer->GetTexture());
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
