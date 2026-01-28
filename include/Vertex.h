#pragma once

#include <vector>
#include "core.h"
#include <tuple>

class Vertex {
	public:
		Vertex();

		glm::vec3 position;
		glm::vec3 normal;
		std::vector<std::tuple<int, float>> skinWeight;
};