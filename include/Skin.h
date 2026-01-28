#pragma once

#include <vector>
#include "core.h"
#include "Vertex.h"
#include "Triangle.h"
#include <tuple>

class Skin {
	public:
		Skin();
		~Skin();

		bool Load(const char* file);
		void Update();
		void Draw(const glm::mat4& viewProjMtx, GLuint shader);

	private:
		std::vector<Vertex*> vertices;
		std::vector<Triangle*> triangles;
		std::vector<std::vector<int>> vertexIndices;
		std::vector<glm::mat4> bindings;
		
		std::vector<glm::mat4> skinMats;
		std::vector<glm::mat4> skinMatsInvT;
		
		std::vector<glm::vec3> newVertices;
		std::vector<glm::vec3> newNormals;
		
		int jointCount;
		int vertexCount;
		int triangleCount;
		
		bool initialized;
};