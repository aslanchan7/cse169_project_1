#pragma once

#include <vector>
#include "core.h"
#include "DOF.h"
#include "Tokenizer.h"
#include "Cube.h"
#include <string>

class Joint {
	public:
		Joint();
		~Joint();

		void Update();
		bool Load(Tokenizer &t);
		void AddChild(Joint*);
		void Draw(const glm::mat4& viewProjMtx, GLuint shader);
		std::vector<DOF> dof;
		std::string name;
		glm::mat4 worldMat;

	private:
		glm::mat4 localMat;
		glm::vec3 jointOffset;
		glm::vec3 boxmin;
		glm::vec3 boxmax;
		glm::vec3 pose;
		Cube* cube;

		// tree data
		std::vector<Joint*> children;
		Joint* parent;
};