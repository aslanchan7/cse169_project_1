#pragma once

#include <vector>
#include "Tokenizer.h"
#include <iostream>
#include "Skeleton.h"
#include <tuple>

class Skin {
public:
    void Update();
    bool Load(const char* file);
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);

private:
    std::vector<glm::vec3> positions; 
    std::vector<glm::vec3> normals;

    std::vector<std::vector<std::tuple<int, float>>> skinWeights; 
    std::vector<glm::ivec3> triangleIndices; 

    std::vector<glm::mat4> bindings;

    int vertexCount;
    int triangleCount;
    int jointCount;

    std::vector<glm::vec3> newPositions;
    std::vector<glm::vec3> newNormals; 
};