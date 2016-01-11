

#include "Renderer.h"

Renderer::Renderer():
	ovrManager()
{
}

    
void Renderer::render(){
	
	if(!window){
		window = std::make_unique<GL::Window>(width, height, "Visualisation Window", GL::WindowStyle::Close);
		GL::Context& gl = window->GetContext();
		scene = std::make_unique<Scene>();
	}
	GL::Context& gl = window->GetContext();

	// Main loop
	ovrManager.printCurrentPose();
	
	GL::Shader vert(GL::ShaderType::Vertex, GLSL(
		in vec3 pos;
		in vec3 normal;
		in vec2 texcoord;
		out vec3 Pos;
		out vec3 Normal;
		out vec2 Texcoord;
		uniform mat4 modelview;
		uniform mat4 projection;
		void main() {
			Pos = modelview * pos;
			Normal = modelview * normal;
			Texcoord = texcoord;
			gl_Position = projection * modelview * vec4( pos, 1.0 );
		}
	));

	GL::Shader frag(GL::ShaderType::Fragment, GLSL(
		in vec3 Pos;
		in vec3 Normal;
		in vec2 Texcoord;
		out vec4 outColor;
		uniform sampler2D texture;
		void main() {
			outColor = texture(texture,Texcoord);
		}
	));
	GL::Program program(vert, frag);

	GL::Event ev;
	if (window->IsOpen())
    {
        while (window->GetEvent(ev));


		gl.Clear();


		glViewport(0,0,width/2,height);

		scene->render(gl,program);



		glViewport(width/2,0,width/2,height);

		scene->render(gl, program);


        window->Present();

	}
	
}
