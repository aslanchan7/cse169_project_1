#pragma once

#include "Vertex.h"

class Triangle {
	public:
		void Draw();
	
	private:
		std::vector<Vertex> vertices;
};