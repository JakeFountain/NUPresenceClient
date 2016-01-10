

#include "Renderer.h"

Renderer::Renderer():
	window(width,height,"Visualisation Window", GL::WindowStyle::Close),
	ovrManager()
	{}
    
void Renderer::run(){
	GL::Context& gl = window.GetContext();
	// Main loop
	bool running = true; 

	Scene scene;
	
    while (running)
    {
		ovrManager.printCurrentPose();
    	
	    GL::Shader vert(GL::ShaderType::Vertex, "#version 150\nin vec2 position; void main() { gl_Position = vec4(position, 0.0, 1.0); }");
	    GL::Shader frag(GL::ShaderType::Fragment, "#version 150\nout vec4 outColor; void main() { outColor = vec4(1.0, 0.0, 0.0, 1.0); }");
	    GL::Program program(vert, frag);

	    GL::Event ev;
	    while (window.IsOpen())
	    {
	        while (window.GetEvent(ev));

	        gl.Clear();

	        glViewport(0,0,width/2,height);
	        scene.render(gl,program);


	        glViewport(width/2,0,width/2,height);
	        scene.render(gl, program);

	        window.Present();
	    }

    }
	
}
