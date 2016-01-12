

#include "Renderer.h"

Renderer::Renderer():
	ovrManager()
{
}

    
void Renderer::render(float t_sec){
	
	if(!window){
		window = std::make_unique<GL::Window>(width, height, "Visualisation Window", GL::WindowStyle::Close);
		GL::Context& gl = window->GetContext();
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
				gl_Position = projection * modelview * vec4(pos, 1.0);
			}
		));

		frag = std::make_unique<GL::Shader>(GL::ShaderType::Fragment, GLSL(
			in vec4 Pos;
			in vec4 Normal;
			in vec2 Texcoord;
			out vec4 outColor;
			uniform sampler2D tex;
			void main() {
				outColor = texture(tex, vec2(Texcoord.x,1-Texcoord.y));
			}
		));

		// std::cout << "Vert compile output: \n" << vert->GetInfoLog() << std::endl;
		// std::cout << "Frag compile output: \n" << frag->GetInfoLog() << std::endl;
		program = std::make_unique<GL::Program>(*vert, *frag);
		// std::cout << "Link output: \n" << program->GetInfoLog() << std::endl;
		
		} catch (GL::CompileException e) {
			std::cout << e.what() << std::endl;
			throw e;
		}

		ovrManager.init();


	}
	GL::Context& gl = window->GetContext();

	// Main loop
	//ovrManager.printCurrentPose();
	


	GL::Event ev;
	if (window->IsOpen())
    {
        while (window->GetEvent(ev));


		gl.Clear();
		
		float camera_period = 10;
		float sin = std::sin(2 * 3.14 * t_sec / camera_period);
		float cos = std::cos(2 * 3.14 * t_sec / camera_period);
		GL::Mat4 origin = GL::Mat4::LookAt(GL::Vec3(2 * cos, 2 * sin, 0), GL::Vec3(0, 0, 0), GL::Vec3(0, 0, 1));

		auto poses = ovrManager.getCurrentPoses();
		
		int eyeNumber = 0;
		for (auto& pose : poses) {
			GL::Mat4 view = pose.view * origin;
			glViewport(eyeNumber * width / 2, 0, width / 2, height);
			scene->render(gl, *program, view, pose.proj);
			eyeNumber++;
		}

        window->Present();

	}
	
}
