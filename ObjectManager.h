/*
Created by Henri Puennel, W19019002.
This file...
*/
#pragma once
#include "GameObject.h"
#include <list>
#include "gametimer.h"

class ObjectManager {
private:
	std::list<GameObject*> pObjectList;
public:
	void AddObject(GameObject* pNewObject);
	void UpdateAll(double gt);
	void RenderAll();
	void DeleteAll();
	void DeleteInactiveObjects();
	void CheckAllCollisions();
	ObjectManager();
	~ObjectManager();
};