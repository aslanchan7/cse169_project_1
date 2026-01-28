#pragma once

#include "Vertex.h"
#include "core.h"

class Triangle {
	public:
		Triangle(std::vector<glm::vec3>& vertices, std::vector<glm::vec3> normals);
		~Triangle();

		void Draw(const glm::mat4& viewProjMtx, GLuint shader);

		//std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> positions;
	private:
		GLuint VAO;
		GLuint VBO_positions, VBO_normals, EBO;

		glm::mat4 model;
		glm::vec3 color;

		// Triangle Information
		std::vector<unsigned int> indices;
};