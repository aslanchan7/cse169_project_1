#pragma once
#include "Tokenizer.h"

class Keyframe {
	public:
		Keyframe();
		~Keyframe();

		bool Load(Tokenizer* tokenizer);

		float time;
		float value;
		char* ruleIn;
		char* ruleOut;

		float tangentIn, tangentOut;
		float A, B, C, D; // Coefficients for cubic Hermite interpolation
		float denominator; // Precomputed denominator for interpolation
};	