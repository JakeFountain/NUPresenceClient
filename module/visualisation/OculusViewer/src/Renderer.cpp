

#include "Renderer.h"

Renderer::Renderer(){
	
}

Renderer::~Renderer(){
    glfwTerminate();
}

void Renderer::init(){
	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "Error initialising GLFW" << std::endl;
		throw std::runtime_error("Error initialising glfw");
	}

	/* Create a windowed mode window and its OpenGL context */
	width = 1920;
	height = 1080;
	window = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(glfwCreateWindow(width, height, "NUPresence Client", nullptr, nullptr), glfwDestroyWindow);

	if (!window)
	{
		glfwTerminate();
		std::cout << "Error initialising window" << std::endl;
		throw std::runtime_error("Error initialising window");
	}

	glfwMakeContextCurrent(window.get());

	GLuint glewResult = glewInit();
	if (glewResult != GLEW_OK) {
		std::cout << "Error initialising GLEW: " << glewGetErrorString(glewResult) << std::endl;
		throw std::runtime_error("Error initialising GLEW");
	}

	glGetError();

	gl = std::make_unique<Graphics>();

	try {
		oglplus::VertexShader vert;
		oglplus::FragmentShader frag;
		vert.Source(oglplus::StrCRef(
			"in vec3 pos; \
			in vec3 normals; \
			in vec2 texcoord; \
			out vec4 Pos; \
			out vec4 Normal; \
			out vec2 Texcoord; \
			uniform mat4 modelview; \
			uniform mat4 projection; \
			void main() { \
				Pos = modelview * vec4(pos,1.0); \
				Normal = modelview * vec4(normals,0.0); \
				Texcoord = texcoord; \
				gl_Position = projection * vec4(Pos.x,-Pos.y, Pos.z, 1.0); \
			}"
			)).Compile();

		frag.Source(oglplus::StrCRef(
			"in vec4 Pos; \
			in vec4 Normal; \
			in vec2 Texcoord; \
			out vec4 outColor; \
			uniform sampler2D tex; \
			void main() { \
				outColor = texture(tex, vec2(Texcoord.x, 1 - Texcoord.y)); \
				/*outColor = vec4(mod(abs(Pos.z),1), 0, 0, 1);*/ \
			}"
			)).Compile();

		gl->program.AttachShader(vert).AttachShader(frag);
		gl->program.Link().Use();
	}
	catch (oglplus::ProgramBuildError err) {
		std::cout << __FILE__ << " : Shader compilation failed - " << err.Log() << std::endl;
	}

	if (gl->program.IsLinked()) {
		std::cout << "Shader linked successfully!" << std::endl;
	}
	else {
		std::cout << "Shader LINKING FAILED!" << std::endl;

	}

	// bool ovrInitialised = ovrManager.init();
	

}

    
bool Renderer::render(float t_sec){
	
	// Main loop	
	if (!window) {
		init();
	}


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
		oglplus::Mat4f poseview;

//		auto poses = ovrManager.getCurrentPoses();
		
		//Draw eye buffers

		int eyeNumber = 0;
		//for (auto& pose : poses) {
			oglplus::Mat4f view = poseview * origin;
			//oglplus::Mat4f view = pose.view * origin;
			//ovrManager.setRenderTarget(gl, OVRManager::RenderTarget(eyeNumber));
			//if(eyeNumber == 0) gl.Clear();
			gl->scene.render(gl->context, gl->program, view, oglplus::Mat4f());
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

		gl->context.ClearColor(0, 0, 1, 1);
		gl->context.Clear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window.get());

        /* Poll for and process events */
        glfwPollEvents();

		if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window.get(), 1);
		}
        return true;
    } else {
    	return false;
    }
}
