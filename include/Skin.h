#pragma once

#include <vector>
#include "core.h"
#include "Vertex.h"
#include "Triangle.h"

class Skin {
	public:
		Skin();
		~Skin();

		bool Load(const char* file);
		void Update();
		void Draw();

	private:
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		std::vector<glm::mat4> bindings;
};