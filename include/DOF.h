#pragma once

class DOF {
	public:
		DOF();

		void SetValue(float val);
		float GetValue() const;
		void SetMinMax(float l, float h);

		float value;
		float min;
		float max;
};