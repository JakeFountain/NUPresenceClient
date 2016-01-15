

#include "Renderer.h"

Renderer::Renderer(){
	glewInit();

    /* Initialize the library */
	if (!glfwInit()) {
		throw std::runtime_error("Error initialising glfw");
	}

	/* Create a windowed mode window and its OpenGL context */
	width = 1920;
	height = 1080;
	window = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(glfwCreateWindow(width, height, "NUPresence Client", nullptr, nullptr), glfwDestroyWindow);

    if (!window)
    {
        glfwTerminate();
		throw std::runtime_error("Error initialising window");
    }
//   vert = std::make_unique<oglplus::Shader>(oglplus::ShaderType::Vertex, GLSL(
	// 	in vec3 pos;
	// 	in vec3 normal;
	// 	in vec2 texcoord;
	// 	out vec4 Pos;
	// 	out vec4 Normal;
	// 	out vec2 Texcoord;
	// 	uniform mat4 modelview;
	// 	uniform mat4 projection;
	// 	void main() {
	// 		Pos = modelview * vec4(pos,1.0);
	// 		Normal = modelview * vec4(normal,0.0);
	// 		Texcoord = texcoord;
	// 		gl_Position = projection * vec4(Pos.x,-Pos.y, Pos.z, 1.0);
	// 	}
	// ));

	// frag = std::make_unique<oglplus::Shader>(oglplus::ShaderType::Fragment, GLSL(
	// 	in vec4 Pos;
	// 	in vec4 Normal;
	// 	in vec2 Texcoord;
	// 	out vec4 outColor;
	// 	uniform sampler2D tex;
	// 	void main() {
	// 		outColor = texture(tex, vec2(Texcoord.x, 1 - Texcoord.y));
	// 		//outColor = vec4(mod(abs(Pos.z),1), 0, 0, 1);
	// 	}
	// ));

	// program = std::make_unique<oglplus::Program>(*vert, *frag);

	// } catch (oglplus::CompileException e) {
	// 	std::cout << e.what() << std::endl;
	// 	throw e;
	// }

	//bool ovrInitialised = ovrManager.init();
	
}

Renderer::~Renderer(){
    glfwTerminate();
}

    
bool Renderer::render(float t_sec){
	
	// Main loop	

    /* Make the window's context current */
    glfwMakeContextCurrent(window.get());

    /* Loop until the user closes the window */
    if (!glfwWindowShouldClose(window.get()))
    {
        /* Render here */
		float camera_period = 10;
		float sin = std::sin(2 * 3.14 * t_sec / camera_period);
		float cos = std::cos(2 * 3.14 * t_sec / camera_period);
		oglplus::Mat4f origin;// = oglplus::Mat4f(oglplus::Vec3f(0, 0, 0), oglplus::Vec3f(sin, cos, 0), oglplus::Vec3f(0, 0, 1));

//		auto poses = ovrManager.getCurrentPoses();
		
		//Draw eye buffers
		int eyeNumber = 0;
		//for (auto& pose : poses) {
			//oglplus::Mat4f view = pose.view * origin;
			//ovrManager.setRenderTarget(gl, OVRManager::RenderTarget(eyeNumber));
			//if(eyeNumber == 0) gl.Clear();
			//scene->render(gl, *program, view, pose.proj);
			//eyeNumber++;
		//}
		
		//Draw to rift
		//bool renderResult = ovrManager.renderToRift();
		//if (!renderResult) {
		//	std::cout << "Rift display failed!" << std::endl;
		//}

		//Draw to mirror
//		gl.BindFramebuffer(); //Bind to screen
		//oglplus method?
		//glViewport(0, 0, width, height);
		//ovrManager.drawMirror(gl);

        /* Swap front and back buffers */
        glfwSwapBuffers(window.get());

        /* Poll for and process events */
        glfwPollEvents();
        return true;
    } else {
    	return false;
    }
}
