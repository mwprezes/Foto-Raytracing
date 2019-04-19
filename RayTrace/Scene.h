#pragma once
#include "Primitive.h"

class Scene
{
private:
	Primitive **primitives;
	int addIndex = 0;
	int maxIndex;
	//array<Primitive, maxIndex> 

public:
	Scene();
	Scene(int numberOfPrimitives);
	~Scene();

	void setPrimitive(int at, Primitive val) {
		primitives[at] = &val;
	}

	Primitive *getPrimitive(int at) {
		if (at <= addIndex)
			return primitives[at];

		Primitive p = Primitive();
		return &p;
	}
	int getMaxIndex() {
		return maxIndex;
	}
	int getAddIndex() {
		return addIndex;
	}

	void addPrimitive(Primitive *shape);
};

