

#include <GL/OOGL.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "RenderMesh.h"


#ifndef NUPRESENCE_GAME_OBJECT_H
#define NUPRESENCE_GAME_OBJECT_H

class GameObject {
public:
	std::vector<GameObject> children;
	std::vector<RenderMesh> meshes;
	// std::vector<Light> lights;
	GL::Mat4 transform;

	GameObject(GL::Mat4 t = GL::Mat4()) : transform(t){};	
	~GameObject(){};	
	
	void addMesh(RenderMesh& mesh){
		meshes.push_back(mesh);
	}
	
	void addChild(GameObject& child){
		children.push_back(child);
	}

	// void addLight(const Light& light){
	// 	lights.push_back(light);
	// }

	void render(GL::Context& gl, GL::Mat4 prevModel, GL::Mat4 view, GL::Mat4 projection/*, std::vector<Light>& outLights*/, GL::Program& shader){
		GL::Mat4 model = prevModel * transform;
		GL::Mat4 modelview = view * model;
		for(auto& child : children){
			child.render(gl, model, view, projection, shader);
		}

		for(auto& mesh : meshes){
			mesh.render(gl, modelview, projection, shader);
		}

		// for(auto& light : lights){
		// 	outLights.push_back(light.transform(model));
		// }
	}


};
#endif