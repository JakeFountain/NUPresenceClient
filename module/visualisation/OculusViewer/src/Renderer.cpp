

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
	
	GL::Shader vert(GL::ShaderType::Vertex, "#version 150\nin vec2 position; void main() { gl_Position = vec4(position, 0.0, 1.0); }");
	GL::Shader frag(GL::ShaderType::Fragment, "#version 150\nout vec4 outColor; void main() { outColor = vec4(1.0, 0.0, 0.0, 1.0); }");
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
