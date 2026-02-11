#include "Keyframe.h"

Keyframe::Keyframe() {
	time = 0.0f;
	value = 0.0f;
	tangentIn = 0.0f;
	tangentOut = 0.0f;
	ruleIn = nullptr;
	ruleOut = nullptr;
	A = B = C = D = 0.0f;
	denominator = 0.0f;
}

Keyframe::~Keyframe() {
}

bool Keyframe::Load(Tokenizer* tokenizer) {
	time = tokenizer->GetFloat();
	value = tokenizer->GetFloat();

	char buffer[256];
	tokenizer->GetToken(buffer);
	if (strcmp(buffer, "flat") == 0) {
		ruleIn = "flat";
	}
	else if (strcmp(buffer, "linear")) {
		ruleIn = "linear";
	}
	else if (strcmp(buffer, "smooth")) {
		ruleIn = "smooth";
	}
	else {
		// Convert buffer to float
		ruleIn = "fixed";
		tangentIn = strtof(buffer, nullptr);
		tangentIn = tangentIn;
	}

	tokenizer->GetToken(buffer);
	if (strcmp(buffer, "flat") == 0) {
		ruleOut = "flat";
	}
	else if (strcmp(buffer, "linear")) {
		ruleOut = "linear";
	}
	else if (strcmp(buffer, "smooth")) {
		ruleOut = "smooth";
	}
	else {
		// Convert buffer to float
		ruleOut = "fixed";
		tangentOut = strtof(buffer, nullptr);
	}

	return true;
}