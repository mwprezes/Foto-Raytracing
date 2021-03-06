#include "Scene.h"



Scene::Scene()
{
	maxIndex = 1;
	primitives = new Primitive*[1];
}

Scene::Scene(int numberOfPrimitives)
{
	primitives = new Primitive*[numberOfPrimitives];
	maxIndex = numberOfPrimitives;
	for (int i = 0; i < maxIndex; i++)
		primitives[i] = new Primitive();
}


Scene::~Scene()
{
	//delete[] primitives;
	for (int i = 0; i < maxIndex; i++) {
		delete[] primitives[i];
	}
	delete[] primitives;
	primitives = NULL;

	for (int i = 0; i < lights.size(); i++) {
		delete[] lights[i];
	}
	lights.clear();
}

void Scene::addPrimitive(Primitive *shape)
{
	if (addIndex < maxIndex) 
	{
		primitives[addIndex] = shape;
		addIndex++;
	}
	else
	{
		//No no
	}
}

void Scene::addLight(LightSource* light)
{
	lights.push_back(light);
}
