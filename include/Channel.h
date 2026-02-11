#pragma once
#include <vector>
#include "Keyframe.h"
#include "Tokenizer.h"
#include "core.h"

class Channel {
	public:
		Channel();
		~Channel();
		
		bool Load(Tokenizer* tokenizer);
		float Evaluate(float time, int lastKeyframeIndex);
		void Precompute();
	
		std::vector<Keyframe*> keyframes;
		int keyframeCount;
		char* extrapolateIn;
		char* extrapolateOut;
	
		const glm::mat4 hermiteBasis = glm::mat4(
			2.0f, -3.0f, 0.0f, 1.0f,
			-2.0f, 3.0f, 0.0f, 0.0f,
			1.0f, -2.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 0.0f
		);
};