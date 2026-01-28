#include "Skin.h"
#include "Skeleton.h"
#include <iostream>

Skin::Skin() {
	vertices = std::vector<Vertex*>{};
	triangles = std::vector<Triangle*>{};
	bindings = std::vector<glm::mat4>{};
	jointCount = 0;
	vertexCount = 0;
	triangleCount = 0;
	initialized = false;
}

Skin::~Skin() {
	for each(Vertex* vertex in vertices)
	{
		delete vertex;
	}
	
	for each(Triangle * triangle in triangles)
	{
		delete triangle;
	}
}

bool Skin::Load(const char* file) {
	Tokenizer tokenizer;
	bool success = tokenizer.Open(file);
	
	tokenizer.FindToken("positions");
	int n = tokenizer.GetInt();
	vertexCount = n;
	tokenizer.FindToken("{");
	for (int i = 0; i < n; i++)
	{
		Vertex* vertex = new Vertex();
		vertex->position = glm::vec3( tokenizer.GetFloat(), tokenizer.GetFloat(), tokenizer.GetFloat() );
		vertices.push_back(vertex);
	}
	tokenizer.FindToken("}");

	tokenizer.FindToken("normals");
	tokenizer.FindToken("{");
	for (int i = 0; i < n; i++)
	{
		vertices[i]->normal = glm::vec3( tokenizer.GetFloat(), tokenizer.GetFloat(), tokenizer.GetFloat() );
	}
	tokenizer.FindToken("}");

	tokenizer.FindToken("skinweights");
	tokenizer.FindToken("{");
	for (int i = 0; i < n; i++)
	{
		int m = tokenizer.GetInt();
		for (int j = 0; j < m; j++)
		{
			int jointIndex = tokenizer.GetInt();
			float weight = tokenizer.GetFloat();
			vertices[i]->skinWeight.push_back(std::tuple<int, float>(jointIndex, weight));
		}
	}
	tokenizer.FindToken("}");

	tokenizer.FindToken("triangles");
	triangleCount = tokenizer.GetInt();
	tokenizer.FindToken("{");
	for (int i = 0; i < triangleCount; i++)
	{
		int idx1 = tokenizer.GetInt();
		int idx2 = tokenizer.GetInt();
		int idx3 = tokenizer.GetInt();

		//std::vector<int> vertexIndices = {};
		std::vector<int> triangleIndices = { idx1, idx2, idx3 };
		vertexIndices.push_back(triangleIndices);
	}
	tokenizer.FindToken("}");

	tokenizer.FindToken("bindings");
	jointCount = tokenizer.GetInt();
	tokenizer.FindToken("{");
	for (int i = 0; i < jointCount; i++)
	{
		tokenizer.FindToken("{");
		float ax = tokenizer.GetFloat();
		float ay = tokenizer.GetFloat();
		float az = tokenizer.GetFloat();

		float bx = tokenizer.GetFloat();
		float by = tokenizer.GetFloat();
		float bz = tokenizer.GetFloat();

		float cx = tokenizer.GetFloat();
		float cy = tokenizer.GetFloat();
		float cz = tokenizer.GetFloat();

		float dx = tokenizer.GetFloat();
		float dy = tokenizer.GetFloat();
		float dz = tokenizer.GetFloat();

		glm::mat4 binding = {
			ax, ay, az, 0.0f,
			bx, by, bz, 0.0f,
			cx, cy, cz, 0.0f,
			dx, dy, dz, 1.0f
		};
		bindings.push_back(binding);
	}
	tokenizer.FindToken("}");

	tokenizer.Close();
	return success;
}

void Skin::Update() {
	skinMats = std::vector<glm::mat4>(jointCount);
	skinMatsInvT = std::vector<glm::mat4>(jointCount);
	newVertices = std::vector<glm::vec3>(vertexCount);
	newNormals = std::vector<glm::vec3>(vertexCount);

	// foreach joint
	for (int i = 0; i < jointCount; i++)
	{
		glm::mat4 jointMat = Skeleton::GetWorldMatrix(i);
		glm::mat4 skinMat = jointMat * glm::inverse(bindings[i]);
		glm::mat4 skinMatInvTran = glm::inverse(glm::transpose(skinMat));

		skinMats[i] = skinMat;
		skinMatsInvT[i] = skinMatInvTran;
	}

	for (int i = 0; i < vertexCount; i++)
	{
		glm::vec3 newPos(0.0f);
		glm::vec3 newNorm(0.0f);

		for (auto weight : vertices[i]->skinWeight) {
			glm::mat4 M = skinMats[std::get<0>(weight)];
			glm::vec4 pos = M * glm::vec4(vertices[i]->position, 1.0f);
			newPos += std::get<1>(weight) * glm::vec3(pos);

			glm::vec4 norm = M * glm::vec4(vertices[i]->normal, 0.0f);
			newNorm += std::get<1>(weight) * glm::vec3(norm);
		}

		newVertices[i] = newPos;
		newNormals[i] = glm::normalize(newNorm);
	}

	if (!initialized) {
		triangles = std::vector<Triangle*>(triangleCount);
		for (int i = 0; i < triangleCount; i++)
		{
			std::vector<glm::vec3> positions = {
				newVertices[vertexIndices[i][0]],
				newVertices[vertexIndices[i][1]],
				newVertices[vertexIndices[i][2]]
			};

			std::vector<glm::vec3> normals = {
				newNormals[vertexIndices[i][0]],
				newNormals[vertexIndices[i][1]],
				newNormals[vertexIndices[i][2]]
			};

			Triangle* triangle = new Triangle(positions, normals);
			triangles[i] = triangle;
		}
		initialized = true;
	}
}

void Skin::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i]->Draw(viewProjMtx, shader);
	}
}