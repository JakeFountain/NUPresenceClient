
#include "Scene.h"


    Scene::Scene(){
		// float vertices[] = {	
	 //        -0.75f,  0.5f,
	 //         0.5f,  0.5f,
	 //         0.5f, -0.5f
	 //    };
	    
	 //    vbo = GL::VertexBuffer(vertices, sizeof(vertices), GL::BufferUsage::StaticDraw);
		GameObject monkey(GL::Mat4().Scale(GL::Vec3(10,10,10)));
		try {
			monkey.addMesh(RenderMesh("../../../assets/monkey.obj", "../../../assets/monkey_texture2.png"));
		} catch (GL::FileException e) {
			std::cout << "File exception: " << std::endl;
		}
		rootObject.addChild(monkey);
    }

    void Scene::render(GL::Context& glContext, GL::Program& shaderProgram, GL::Mat4 view, GL::Mat4 projection){
    	glContext.UseProgram(shaderProgram);


	    rootObject.render(glContext, GL::Mat4(), view, projection, shaderProgram);

    }