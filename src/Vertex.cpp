#include "Vertex.h"

Vertex::Vertex() {
	// Initialize position, normal, and skinWeight as empty vectors
	position = glm::vec3(0.0f);
	normal = glm::vec3(0.0f);
	skinWeight = std::vector<std::tuple<int, float>>{};
}
