#include "Joint.h"
#include "Window.h"
#include <iostream>

Joint::Joint() {
	// Initialize initial values
	dof = std::vector<DOF>(3);
	jointOffset = glm::vec3(0.,0.,0.);
	boxmin = glm::vec3(-0.1, -0.1, -0.1);
	boxmax = glm::vec3(0.1, 0.1, 0.1);
	dof[0].SetMinMax(-100000., 100000.);
	dof[1].SetMinMax(-100000., 100000.);
	dof[2].SetMinMax(-100000., 100000.);
	pose = glm::vec3(0.,0.,0.);
	children = std::vector<Joint*>();
	parent = nullptr;
	localMat = glm::mat4(1.0f);
	worldMat = glm::mat4(1.0f);
	cube = new Cube(boxmin, boxmax);
	name = "LOL";
	Window::joints.push_back(this);
}

Joint::~Joint() {
	delete cube;
}

bool Joint::Load(Tokenizer &token) {
	char buffer[256];
	token.GetToken(buffer);
	this->name = buffer;

	token.FindToken("{");

	while(true) {
		char temp[256];
		token.GetToken(temp);

		if(strcmp(temp, "offset") == 0) {
			float x = token.GetFloat();
			float y = token.GetFloat();
			float z = token.GetFloat();
			jointOffset = glm::vec3(x,y,z);
		} else if(strcmp(temp, "boxmin") == 0) {
			float x = token.GetFloat();
			float y = token.GetFloat();
			float z = token.GetFloat();
			boxmin = glm::vec3(x,y,z);
		} else if(strcmp(temp, "boxmax") == 0) {
			float x = token.GetFloat();
			float y = token.GetFloat();
			float z = token.GetFloat();
			boxmax = glm::vec3(x, y, z);
		} else if(strcmp(temp, "rotxlimit") == 0) {
			float min = token.GetFloat();
			float max = token.GetFloat();
			dof[0].SetMinMax(min, max);
		} else if(strcmp(temp, "rotylimit") == 0) {
			float min = token.GetFloat();
			float max = token.GetFloat();
			dof[1].SetMinMax(min, max);
		} else if(strcmp(temp, "rotzlimit") == 0) {
			float min = token.GetFloat();
			float max = token.GetFloat();
			dof[2].SetMinMax(min, max);
		} else if(strcmp(temp, "pose") == 0) {
			dof[0].SetValue(token.GetFloat());
			dof[1].SetValue(token.GetFloat());
			dof[2].SetValue(token.GetFloat());
		} else if(strcmp(temp, "balljoint") == 0) {
			Joint* newJoint = new Joint();
			AddChild(newJoint);
			newJoint->Load(token);
		} else if(strcmp(temp, "}") == 0) {
			cube = new Cube(boxmin, boxmax);

			return true;
		} else {
			token.SkipLine();
		}
	}
}

void Joint::AddChild(Joint* child) {
	children.push_back(child);
	child->parent = this;
}

void Joint::Update() {
	// calculate localMat
	glm::mat4 translateMat(1.0f);
	translateMat[3] = glm::vec4(jointOffset, 1.0f);

	glm::mat4 xRotMat(1.0f);
	xRotMat[1][1] = cos(dof[0].GetValue());
	xRotMat[2][1] = -sin(dof[0].GetValue());
	xRotMat[1][2] = sin(dof[0].GetValue());
	xRotMat[2][2] = cos(dof[0].GetValue());

	glm::mat4 yRotMat(1.0f);
	yRotMat[0][0] = cos(dof[1].GetValue());
	yRotMat[2][0] = sin(dof[1].GetValue());
	yRotMat[0][2] = -sin(dof[1].GetValue());
	yRotMat[2][2] = cos(dof[1].GetValue());

	glm::mat4 zRotMat(1.0f);
	zRotMat[0][0] = cos(dof[2].GetValue());
	zRotMat[1][0] = -sin(dof[2].GetValue());
	zRotMat[0][1] = sin(dof[2].GetValue());
	zRotMat[1][1] = cos(dof[2].GetValue());

	localMat = translateMat * zRotMat * yRotMat * xRotMat;

	// calculate worldMat
	if (parent == nullptr) {
		worldMat = localMat;
	}
	else {
		worldMat = parent->worldMat * localMat;
	}

	for each(Joint* child in children)
	{
		child->Update();
	}
}

void Joint::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
	cube->draw(viewProjMtx * worldMat, shader);

	for each(Joint* child in children)
	{
		child->Draw(viewProjMtx, shader);
	}
}