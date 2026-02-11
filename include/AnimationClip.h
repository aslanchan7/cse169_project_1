#pragma once
#include <vector>
#include "Channel.h"
#include "Joint.h"
#include "core.h"
#include "core.h"

class AnimationClip {
	public:
		AnimationClip();
		~AnimationClip();

		bool Load(const char* fileName);
		void Evaluate(float time, std::vector<Joint*> joints);

		float timeStart;
		float timeEnd;
		int numChannels;
		std::vector<Channel*> channels;
};