
#include "Scene.h"


    Scene::Scene(){
		float vertices[] = {	
	        -0.75f,  0.5f,
	         0.5f,  0.5f,
	         0.5f, -0.5f
	    };
	    
	    vbo = GL::VertexBuffer(vertices, sizeof(vertices), GL::BufferUsage::StaticDraw);

    }

    void Scene::render(GL::Context& glContext, GL::Program& shaderProgram){//, GL::Mat4 view, GL::Mat4 projection){
    	glContext.UseProgram(shaderProgram);
	    
	    //rootObject.render(GL::Mat4(), view, projection, shaderProgram);
	    vao.BindAttribute(shaderProgram.GetAttribute("position"), vbo, GL::Type::Float, 2, 0, 0);

		glContext.DrawArrays(vao, GL::Primitive::Triangles, 0, 3);


    }