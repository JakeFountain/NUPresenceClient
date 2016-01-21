

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
		width = 1920;
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

		context =  std::make_unique<GL::Context>(24,32,8,1,GetDC(glfwGetWin32Window(window.get())));
		scene = std::make_unique<Scene>();
		try {

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

	// Main loop
	//ovrManager.printCurrentPose();
	context->ClearColor(GL::Color(255, 0, 0, 255));


	GL::Event ev;
	if (!glfwWindowShouldClose(window.get()))
    {
        //while (window->GetEvent(ev));


		context->Clear();
		
		float camera_period = 10;
		float sin = std::sin(2 * 3.14 * t_sec / camera_period);
		float cos = std::cos(2 * 3.14 * t_sec / camera_period);
		GL::Mat4 origin = GL::Mat4::LookAt(GL::Vec3(0, 0, 0), GL::Vec3(sin, cos, 0), GL::Vec3(0, 0, 1));

		auto poses = ovrManager.getCurrentPoses();
		
		//Draw eye buffers
		int eyeNumber = 0;
		for (auto& pose : poses) {
			GL::Mat4 view = pose.view * origin;
			ovrManager.setRenderTarget(*context, OVRManager::RenderTarget(eyeNumber));
			if(eyeNumber == 0) context->Clear();
			scene->render(*context, *program, view, pose.proj);
			eyeNumber++;
		}
		
		//Draw to rift
		bool renderResult = ovrManager.renderToRift();
		if (!renderResult) {
			std::cout << "Rift display failed!" << std::endl;
		}

		//Draw to mirror
		context->BindFramebuffer(); //Bind to screen
		context->ClearColor(GL::Color(t_sec * 255 / 10,0,0,255));
		context->Clear();
		glViewport(0, 0, width, height);
		ovrManager.drawMirror(*context);

		glfwSwapBuffers(window.get());

		glfwPollEvents();

		if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) > 0) {
			glfwSetWindowShouldClose(window.get(), 1);
		}
		return true;
	}
	return false;

	
}
