#include "DOF.h"
#include "core.h"

DOF::DOF() {
	value = 0.;
	min = -3.0f;
	max = 3.0f;
}

void DOF::SetValue(float val) {
	value = glm::clamp(val, min, max);
}

float DOF::GetValue() const {
	return value;
}

void DOF::SetMinMax(float l, float h) {
	min = l;
	max = h;
}