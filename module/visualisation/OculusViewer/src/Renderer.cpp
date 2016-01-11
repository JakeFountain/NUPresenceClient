

#include "Renderer.h"

Renderer::Renderer():
	window(width,height,"Visualisation Window", GL::WindowStyle::Close),
	ovrManager()
	{	std::cout << __LINE__ << std::endl;
}
    
void Renderer::render(){
	std::cout << __LINE__ << std::endl;
	GL::Context& gl = window.GetContext();
	// Main loop
	std::cout << __LINE__ << std::endl;
	ovrManager.printCurrentPose();
	
	std::cout << __LINE__ << std::endl;
	GL::Shader vert(GL::ShaderType::Vertex, "#version 150\nin vec2 position; void main() { gl_Position = vec4(position, 0.0, 1.0); }");
	std::cout << __LINE__ << std::endl;
	GL::Shader frag(GL::ShaderType::Fragment, "#version 150\nout vec4 outColor; void main() { outColor = vec4(1.0, 0.0, 0.0, 1.0); }");
	std::cout << __LINE__ << std::endl;
	GL::Program program(vert, frag);

	std::cout << __LINE__ << std::endl;
	GL::Event ev;
	std::cout << __LINE__ << std::endl;
	if (window.IsOpen())
    {
        while (window.GetEvent(ev));

		std::cout << __LINE__ << std::endl;
		gl.Clear();

		std::cout << __LINE__ << std::endl;
		glViewport(0,0,width/2,height);
		std::cout << __LINE__ << std::endl;
		scene.render(gl,program);


		std::cout << __LINE__ << std::endl;
		glViewport(width/2,0,width/2,height);
		std::cout << __LINE__ << std::endl;
		scene.render(gl, program);
		std::cout << __LINE__ << std::endl;

        window.Present();
		std::cout << __LINE__ << std::endl;
	}
	
}
