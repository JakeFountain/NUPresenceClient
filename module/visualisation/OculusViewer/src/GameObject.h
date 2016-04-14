

#include <GL/OOGL.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "RenderMesh.h"
#include <memory>


#ifndef MODULES_VISUALISATION_NUPRESENCE_GAME_OBJECT_H
#define MODULES_VISUALISATION_NUPRESENCE_GAME_OBJECT_H

namespace module {
namespace visualisation {

	class GameObject {
	public:
		std::vector<std::shared_ptr<GameObject>> children;
		std::vector<std::shared_ptr<RenderMesh>> meshes;
		// std::vector<Light> lights;
		//transform maps from object space to world space
		GL::Mat4 transform;

		GameObject(GL::Mat4 t = GL::Mat4()) : transform(t){};	
		~GameObject(){};	
		
		void addMesh(const std::shared_ptr<RenderMesh>& mesh){
			meshes.push_back(mesh);
		}
		
		void addChild(const std::shared_ptr<GameObject>& child){
			children.push_back(child);
		}

		// void addLight(const Light& light){
		// 	lights.push_back(light);
		// }

		void render(GL::Context& gl, GL::Mat4 prevModel, GL::Mat4 view, GL::Mat4 projection/*, std::vector<Light>& outLights*/, GL::Program& shader){
			GL::Mat4 model = prevModel * transform;
			GL::Mat4 modelview = view * model;
			for(auto& child : children){
				child->render(gl, model, view, projection, shader);
			}

			for(auto& mesh : meshes){
				mesh->render(gl, modelview, projection, shader);
			}

			// for(auto& light : lights){
			// 	outLights.push_back(light.transform(model));
			// }
		}


	};

	}
}
#endif