

#include "Renderer.h"

Renderer::Renderer():
	ovrManager()
{
}

    
void Renderer::render(float t_sec){
	
	if(!window){
		auto resolution = ovrManager.getMirrorResolution();
		width = resolution.w;
		height = resolution.h;
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
				gl_Position = projection * Pos;
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
		GL::Mat4 origin;//GL::Mat4::LookAt(GL::Vec3(0, 0, 0), GL::Vec3(sin, cos, 0), GL::Vec3(0, 0, 1));

		auto poses = ovrManager.getCurrentPoses();
		
		//Draw eye buffers
		int eyeNumber = 0;
		for (auto& pose : poses) {
			GL::Mat4 view = pose.view * origin;
			ovrManager.setRenderTarget(gl, OVRManager::RenderTarget(eyeNumber));
			if(eyeNumber == 0) gl.Clear();
			scene->render(gl, *program, view, pose.proj);
			eyeNumber++;
		}
		
		//Draw to rift
		ovrManager.renderToRift();

		//Draw to mirror
		//gl.BindFramebuffer(); //Bind to screen
		//glViewport(0, 0, width, height);

        window->Present();

	}
	
}
