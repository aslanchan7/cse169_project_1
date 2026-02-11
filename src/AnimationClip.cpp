#include "AnimationClip.h"
#include <iostream>

AnimationClip::AnimationClip() {
	timeStart = 0.0f;
	timeEnd = 0.0f;
	numChannels = 0;
}

AnimationClip::~AnimationClip() {
	for (int i = 0; i < numChannels; i++) {
		delete channels[i];
	}
}

bool AnimationClip::Load(const char* fileName) {
	Tokenizer tokenizer;
	tokenizer.Open(fileName);

	tokenizer.FindToken("range");
	timeStart = tokenizer.GetFloat();
	timeEnd = tokenizer.GetFloat();

	tokenizer.FindToken("numchannels");
	numChannels = tokenizer.GetInt();

	tokenizer.FindToken("channel");
	for (int i = 0; i < numChannels; i++) {
		Channel* channel = new Channel();
		bool loaded = channel->Load(&tokenizer);
		
		if (!loaded) {
			delete channel;
			std::cout << "Failed to load channel " << i << std::endl;
			return false;
		}

		channels.push_back(channel);
	}

	tokenizer.Close();

	return true;
}

void AnimationClip::Evaluate(float time, std::vector<Joint*> joints, std::vector<int> channelLastKeyframeIdx) {
	// Set root offset
	//float x = channels[0]->Evaluate(time, channelLastKeyframeIdx[0]);
	//float y = channels[1]->Evaluate(time, channelLastKeyframeIdx[1]);
	//float z = channels[2]->Evaluate(time, channelLastKeyframeIdx[2]);
	//glm::vec3 rootOffset = glm::vec3(x, y, z);
	//joints[0]->jointOffset = rootOffset;

	// Set joint DOF values
	//for (int i = 1; i < joints.size(); i++) {
	//for (int i = 8; i < 13; i++) { // for right side legs only
	for (int i = 8; i < 9; i++) {
		float x = channels[i * 3]->Evaluate(time, channelLastKeyframeIdx[i * 3]);
		float y = channels[i * 3 + 1]->Evaluate(time, channelLastKeyframeIdx[i * 3 + 1]);
		float z = channels[i * 3 + 2]->Evaluate(time, channelLastKeyframeIdx[i * 3 + 2]);
		//joints[i]->dof[0]->SetValue(x);
		//joints[i]->dof[1]->SetValue(y);
		//joints[i]->dof[2]->SetValue(z);
		joints[i]->dof[0]->SetValue(-0.541324f);
		joints[i]->dof[1]->SetValue(0.718820f);
		joints[i]->dof[2]->SetValue(1.467731f);
	}
}