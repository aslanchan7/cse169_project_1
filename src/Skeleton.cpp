#include <iostream>
#include "Skeleton.h"
#include "Tokenizer.h"
#include "Window.h"

Skeleton::Skeleton() {
	root = NULL;
}

Skeleton::~Skeleton() {
	delete root;
}

bool Skeleton::Load(const char* file) {
	Tokenizer tokenizer;
	bool success = tokenizer.Open(file);
	tokenizer.FindToken("balljoint");

	root = new Joint();
	root->Load(tokenizer);

	tokenizer.Close();
	return success;
}

void Skeleton::Update() {
	root->Update();
}

void Skeleton::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
	root->Draw(viewProjMtx, shader);
}

glm::mat4 Skeleton::GetWorldMatrix(int joint) {
	return Window::joints[joint]->worldMat;
}
