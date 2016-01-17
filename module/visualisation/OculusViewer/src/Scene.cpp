
#include "Scene.h"


    Scene::Scene(){
		// float vertices[] = {	
	 //        -0.75f,  0.5f,
	 //         0.5f,  0.5f,
	 //         0.5f, -0.5f
	 //    };
	    
	 //    vbo = oglplus::VertexBuffer(vertices, sizeof(vertices), oglplus::BufferUsage::StaticDraw);
		GameObject monkey;
		monkey.addMesh(RenderMesh("../../../assets/monkey", "../../../assets/monkey_texture2.png"));
		rootObject.addChild(monkey);
    }

    void Scene::render(oglplus::Context& glContext, oglplus::Program& shaderProgram, oglplus::Mat4 view, oglplus::Mat4 projection){
    	//glContext.UseProgram(shaderProgram);


	    rootObject.render(glContext, oglplus::Mat4f(), view, projection, shaderProgram);

    }