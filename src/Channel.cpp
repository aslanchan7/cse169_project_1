#include "Channel.h"
#include <iostream>

Channel::Channel() {
	
}

Channel::~Channel() {
	for (auto keyframe : keyframes) {
		delete keyframe;
	}
}

bool Channel::Load(Tokenizer* tokenizer) {
	char buffer[256];

	tokenizer->FindToken("extrapolate");
	// Get extrapolateIn
	tokenizer->GetToken(buffer);
	if (strcmp(buffer, "constant") == 0) {
		extrapolateIn = "constant";
	}
	else if (strcmp(buffer, "linear") == 0) {
		extrapolateIn = "linear";
	}
	else if (strcmp(buffer, "cycle") == 0) {
		extrapolateIn = "cycle";
	}
	else if (strcmp(buffer, "cycle_offset") == 0) {
		extrapolateIn = "cycle_offset";
	}
	else if (strcmp(buffer, "bounce") == 0) {
		extrapolateIn = "bounce";
	}
	else {
		std::cout << "Invalid extrapolateIn Mode: " << buffer << std::endl;
		return false;
	}

	// Get extrapolateOut
	tokenizer->GetToken(buffer);
	if (strcmp(buffer, "constant") == 0) {
		extrapolateOut = "constant";
	}
	else if (strcmp(buffer, "linear") == 0) {
		extrapolateOut = "linear";
	}
	else if (strcmp(buffer, "cycle") == 0) {
		extrapolateOut = "cycle";
	}
	else if (strcmp(buffer, "cycle_offset") == 0) {
		extrapolateOut = "cycle_offset";
	}
	else if (strcmp(buffer, "bounce") == 0) {
		extrapolateOut = "bounce";
	}
	else {
		std::cout << "Invalid extrapolateOut Mode: " << buffer << std::endl;
		return false;
	}

	tokenizer->FindToken("keys");
	keyframeCount = tokenizer->GetInt();
	tokenizer->FindToken("{");
	for (int i = 0; i < keyframeCount; i++)
	{
		Keyframe* newKeyframe = new Keyframe();
		bool loaded = newKeyframe->Load(tokenizer);
		if (!loaded) {
			delete newKeyframe;
			std::cout << "Failed to load keyframe " << i << std::endl;
			return false;
		}
		keyframes.push_back(newKeyframe);
	}

	this->Precompute();

	return true;
}

float Channel::Evaluate(float time, int lastKeyframeIndex) {
	if (keyframeCount == 0) {
		std::cout << "No keyframes in channel" << std::endl;
		return 0.0f;
	}
	else if (keyframeCount == 1) {
		return keyframes[0]->value;
	}

	if (time < keyframes[0]->time) {
		// use extrapolateIn
		if (strcmp(extrapolateIn, "constant") == 0) {
			return keyframes[0]->value;
		}
		else if (strcmp(extrapolateIn, "linear") == 0) {
			float slope = (keyframes[1]->value - keyframes[0]->value) / (keyframes[1]->time - keyframes[0]->time);
			return keyframes[0]->value + slope * (time - keyframes[0]->time);
		}
		else if (strcmp(extrapolateIn, "cycle") == 0) {
			float x = fmod(time - keyframes[0]->time, keyframes[keyframeCount - 1]->time - keyframes[0]->time);
			if (x < 0) {
				x += keyframes[keyframeCount - 1]->time - keyframes[0]->time;
			}
			return Evaluate(x + keyframes[0]->time, lastKeyframeIndex);
		}
		else if (strcmp(extrapolateIn, "cycle_offset") == 0) {
			float x = fmod(time - keyframes[0]->time, keyframes[keyframeCount - 1]->time - keyframes[0]->time);
			if (x < 0) {
				x += keyframes[keyframeCount - 1]->time - keyframes[0]->time;
			}
			int cycleCount = floor((keyframes[0]->time - time) / (keyframes[keyframeCount - 1]->time - keyframes[0]->time));
			return Evaluate(x + keyframes[0]->time, lastKeyframeIndex) + cycleCount * (keyframes[keyframeCount - 1]->value - keyframes[0]->value);
		}
	}
	else if (time > keyframes[keyframeCount - 1]->time) {
		// use extrapolateOut
		if (strcmp(extrapolateOut, "constant") == 0) {
			return keyframes[keyframeCount - 1]->value;
		}
		else if (strcmp(extrapolateOut, "linear") == 0) {
			float slope = (keyframes[keyframeCount - 1]->value - keyframes[keyframeCount - 2]->value) / (keyframes[keyframeCount - 1]->time - keyframes[keyframeCount - 2]->time);
			return keyframes[keyframeCount - 1]->value + slope * (time - keyframes[keyframeCount - 1]->time);
		}
		else if (strcmp(extrapolateOut, "cycle") == 0) {
			float x = fmod(time - keyframes[0]->time, keyframes[keyframeCount - 1]->time - keyframes[0]->time);
			if (x < 0) {
				x += keyframes[keyframeCount - 1]->time - keyframes[0]->time;
			}
			return Evaluate(x + keyframes[0]->time, lastKeyframeIndex);
		}
		else if (strcmp(extrapolateOut, "cycle_offset") == 0) {
			float x = fmod(time - keyframes[0]->time, keyframes[keyframeCount - 1]->time - keyframes[0]->time);
			if (x < 0) {
				x += keyframes[keyframeCount - 1]->time - keyframes[0]->time;
			}
			int cycleCount = floor((time - keyframes[keyframeCount - 1]->time) / (keyframes[keyframeCount - 1]->time - keyframes[0]->time)) + 1;
			return Evaluate(x + keyframes[0]->time, lastKeyframeIndex) + cycleCount * (keyframes[keyframeCount - 1]->value - keyframes[0]->value);
		}
	}
	else {
		for (int i = 0; i < keyframeCount - 1; i++)
		{
			if (time >= keyframes[i]->time && time < keyframes[i + 1]->time) {
				Keyframe* kf = keyframes[i];
				float u = (time - kf->time) * kf->denominator;
				std::cout << "i: " << i << std::endl;
				std::cout << "time: " << time << std::endl;
				std::cout << "value: " << (kf->D + u * (kf->C + u * (kf->B + u * (kf->A)))) << std::endl;
				return kf->D + u * (kf->C + u * (kf->B + u * (kf->A)));
			}
		}
	}
}

void Channel::Precompute() {
	// Compute tangents from rules
	for (int i = 0; i < keyframeCount; i++)
	{
		// Compute tangentIn
		if (strcmp(keyframes[i]->ruleIn, "flat") == 0) {
			keyframes[i]->tangentIn = 0.0f;
		}
		else if (strcmp(keyframes[i]->ruleIn, "linear") == 0) {
			// special case for first keyframe
			if (i == 0) {
				keyframes[i]->tangentIn = 0.0f;
			}
			else {
				keyframes[i]->tangentIn = (keyframes[i]->value - keyframes[i - 1]->value) / (keyframes[i]->time - keyframes[i - 1]->time);
			}
		}
		else if (strcmp(keyframes[i]->ruleIn, "smooth") == 0) {
			// special case for first & last keyframe
			if (i == 0) {
				keyframes[i]->tangentIn = 0.0f;
			}
			else if (i == keyframeCount - 1) {
				keyframes[i]->tangentIn = (keyframes[i]->value - keyframes[i - 1]->value) / (keyframes[i]->time - keyframes[i - 1]->time);
			}
			else {
				keyframes[i]->tangentIn = (keyframes[i + 1]->value - keyframes[i - 1]->value) / (keyframes[i + 1]->time - keyframes[i - 1]->time);
			}
		}

		// Compute tangentOut
		if (strcmp(keyframes[i]->ruleOut, "flat") == 0) {
			keyframes[i]->tangentOut = 0.0f;
		}
		else if (strcmp(keyframes[i]->ruleOut, "linear") == 0) {
			// special case for last keyframe
			if (i == keyframeCount - 1) {
				keyframes[i]->tangentOut = 0.0f;
			}
			else {
				keyframes[i]->tangentOut = (keyframes[i + 1]->value - keyframes[i]->value) / (keyframes[i + 1]->time - keyframes[i]->time);
			}
		}
		else if (strcmp(keyframes[i]->ruleOut, "smooth") == 0) {
			// special case for first & last keyframe
			if (i == 0) {
				keyframes[i]->tangentOut = (keyframes[i + 1]->value - keyframes[i]->value) / (keyframes[i + 1]->time - keyframes[i]->time);
			}
			else if (i == keyframeCount - 1) {
				keyframes[i]->tangentOut = 0.0f;
			}
			else {
				keyframes[i]->tangentOut = (keyframes[i + 1]->value - keyframes[i - 1]->value) / (keyframes[i + 1]->time - keyframes[i - 1]->time);
			}
		}
	}

	// Precompute cubic Hermite coefficients
	for (int i = 0; i < keyframeCount - 1; i++)
	{
		float t0 = keyframes[i]->time;
		float t1 = keyframes[i + 1]->time;

		float p0 = keyframes[i]->value;
		float p1 = keyframes[i + 1]->value;

		float v0 = keyframes[i]->tangentOut;
		float v1 = keyframes[i + 1]->tangentIn;

		glm::vec4 values = hermiteBasis * glm::vec4(p0, p1, v0 * (t1 - t0), v1 * (t1 - t0));
		keyframes[i]->A = values.x;
		keyframes[i]->B = values.y;
		keyframes[i]->C = values.z;
		keyframes[i]->D = values.w;

		keyframes[i]->denominator = 1.0f / (t1 - t0);
	}
}