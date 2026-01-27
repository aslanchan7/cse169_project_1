#pragma once

#include <vector>

class Vertex {
	private:
		std::vector<float> position;
		std::vector<float> normal;
		std::vector<std::tuple<int, float>> skinWeight;
};