#pragma once
#include "AnimationClip.h"
#include <vector>

class AnimationPlayer {
	public:
		AnimationPlayer();
		~AnimationPlayer();

		void Update(float deltaTime);
		void SetClip(AnimationClip* c);
		
		float time;
		AnimationClip* clip;
		std::vector<int> channelLastKeyframeIdx;
};