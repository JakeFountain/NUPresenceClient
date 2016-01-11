
#include "Scene.h"


    Scene::Scene(){
		// float vertices[] = {	
	 //        -0.75f,  0.5f,
	 //         0.5f,  0.5f,
	 //         0.5f, -0.5f
	 //    };
	    
	 //    vbo = GL::VertexBuffer(vertices, sizeof(vertices), GL::BufferUsage::StaticDraw);
		GameObject monkey;
		monkey.addMesh(RenderMesh("../../../assets/monkey.obj", "../../../assets/monkey_texture.jpg"));
		rootObject.addChild(monkey);
    }

    void Scene::render(GL::Context& glContext, GL::Program& shaderProgram){//, GL::Mat4 view, GL::Mat4 projection){
    	glContext.UseProgram(shaderProgram);

		GL::Mat4 view = GL::Mat4::LookAt(GL::Vec3(1,1,1), GL::Vec3(0,0,0),GL::Vec3(0,0,1));
		GL::Mat4 projection = GL::Mat4::Perspective(1.5,1,0.01,100);
	    
	    rootObject.render(glContext, GL::Mat4(), view, projection, shaderProgram);


    }