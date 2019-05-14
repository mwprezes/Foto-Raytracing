#pragma once
#include "Primitive.h"
#include "LightSource.h"
#include <vector>

class Scene
{
private:
	Primitive **primitives;
	int addIndex = 0;
	int maxIndex;
	//array<Primitive, maxIndex> 

	std::vector<LightSource*> lights;

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
	LightSource* getLight(int at) {
		return lights[at];
	}
	std::vector<LightSource*> getLights() {
		return lights;
	}
	int getMaxIndex() {
		return maxIndex;
	}
	int getAddIndex() {
		return addIndex;
	}

	void addPrimitive(Primitive *shape);
	void addLight(LightSource* light);
};

