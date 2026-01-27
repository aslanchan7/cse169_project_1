#include "Skin.h"

Skin::Skin() {
	vertices = {};
	triangles = {};
	bindings = {};
}

Skin::~Skin() {
	// TODO: Destructor
}

bool Skin::Load(const char* file) {
	// TODO: Load .skin file
}

void Skin::Update() {
	// TODO: 
		// Compute Skinning Matrix for each joint M_i
		// Compute blended world space positions & normals
}

void Skin::Draw() {
	// TODO: Draw triangles using transformed positions & normals
}