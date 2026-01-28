#include "Skin.h"

void Skin::Update() {

    std::vector<glm::mat4> skinMats(jointCount);
	std::vector<glm::mat4> skinMatInvT(jointCount);
    for (int i = 0; i < jointCount; i++) {
        glm::mat4 worldMat = Skeleton::GetWorldMatrix(i);
        glm::mat4 inverseBindingMat = glm::inverse(bindings[i]);
        skinMats[i] = worldMat * inverseBindingMat;
        glm::mat4 skinMatInvTran = glm::inverse(glm::transpose(skinMats[i]));
		skinMatInvT[i] = skinMatInvTran;
    }

    for (int i = 0; i < vertexCount; i++) {
        glm::vec3 newPos(0.0f);
        glm::vec3 newNorm(0.0f);

        for (auto weight : skinWeights[i]) {
            glm::vec4 pos = skinMats[std::get<0>(weight)] * glm::vec4(positions[i], 1.0f);
            newPos += std::get<1>(weight) * glm::vec3(pos);

            glm::vec4 norm = skinMatInvT[std::get<0>(weight)] * glm::vec4(normals[i], 0.0f);
            newNorm += std::get<1>(weight) * glm::vec3(norm);

        }
        newPositions[i] = newPos;
        newNormals[i] = glm::normalize(newNorm);
    }
}

void Skin::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // Draw triangles using transformed positions & normals

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&viewProjMtx[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Uses GL_TRIANGLE instead of triangle class
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < triangleCount; i++) {
        int v0 = triangleIndices[i].x;
        int v1 = triangleIndices[i].y;
        int v2 = triangleIndices[i].z;
        //Vertex 0
        glNormal3fv(&newNormals[v0][0]);
        glVertex3fv(&newPositions[v0][0]);

        //Vertex 1 
        glNormal3fv(&newNormals[v1][0]);
        glVertex3fv(&newPositions[v1][0]);

        //Vertex 2
        glNormal3fv(&newNormals[v2][0]);
        glVertex3fv(&newPositions[v2][0]);
    }

    glEnd();
}

bool Skin::Load(const char* file) {
    Tokenizer tokenizer;
    tokenizer.Open(file);

    tokenizer.FindToken("positions");
    vertexCount = tokenizer.GetInt();
    tokenizer.FindToken("{");
	positions.resize(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        positions[i].x = tokenizer.GetFloat();
        positions[i].y = tokenizer.GetFloat();
        positions[i].z = tokenizer.GetFloat();
    }
    tokenizer.FindToken("}");

    tokenizer.FindToken("normals");
    vertexCount = tokenizer.GetInt();
    tokenizer.FindToken("{");
    normals.resize(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        normals[i] = glm::vec3(tokenizer.GetFloat(), tokenizer.GetFloat(), tokenizer.GetFloat());
    }
    tokenizer.FindToken("}");

    tokenizer.FindToken("skinweights");
    vertexCount = tokenizer.GetInt();
    tokenizer.FindToken("{");
    skinWeights.resize(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        int m = tokenizer.GetInt();
        skinWeights[i].resize(m);
        for (int j = 0; j < m; j++)
        {
            int jointIndex = tokenizer.GetInt();
            float weight = tokenizer.GetFloat();
            std::tuple<int, float> skinWeight = std::tuple<int, float>(jointIndex, weight);
            skinWeights[i][j] = skinWeight;
        }
    }
    tokenizer.FindToken("}");

    tokenizer.FindToken("triangles");
    triangleCount = tokenizer.GetInt();
    tokenizer.FindToken("{");
	triangleIndices.resize(triangleCount);
    for (int i = 0; i < triangleCount; i++)
    {
        int idx1 = tokenizer.GetInt();
        int idx2 = tokenizer.GetInt();
        int idx3 = tokenizer.GetInt();

		glm::ivec3 triangleIndex = glm::ivec3(idx1, idx2, idx3);
        triangleIndices[i] = triangleIndex;
    }
    tokenizer.FindToken("}");

    tokenizer.FindToken("bindings");
    jointCount = tokenizer.GetInt();
    tokenizer.FindToken("{");
	bindings.resize(jointCount);
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
        bindings[i] = binding;
		tokenizer.FindToken("}");
    }
    tokenizer.FindToken("}");

    //deformed arrays
    newPositions.resize(vertexCount);
    newNormals.resize(vertexCount);

    tokenizer.Close();
    return true;
}