#include "DOF.h"
#include "core.h"

DOF::DOF() {
	value = 0.;
	min = -100000.;
	max = 100000.;
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