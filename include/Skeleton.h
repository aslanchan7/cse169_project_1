#pragma once

#include "Joint.h"

class Skeleton {
	public:
		Skeleton();
		~Skeleton();

		bool Load(const char* file);
		void Update();
		void Draw(const glm::mat4& viewProjMtx, GLuint shader);

	private:
		Joint* root;	
};