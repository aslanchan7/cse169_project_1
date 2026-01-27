#pragma once

#include "Joint.h"
#include "core.h"

class Skeleton {
	public:
		Skeleton();
		~Skeleton();

		bool Load(const char* file);
		void Update();
		void Draw(const glm::mat4& viewProjMtx, GLuint shader);

		glm::mat4 GetWorldMatrix(int joint);

	private:
		Joint* root;	
};