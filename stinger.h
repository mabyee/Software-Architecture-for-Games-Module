/*
Created by Henri Puennel, W19019002.
This file...
*/
#pragma once
#include "GameObject.h"
#include "ObjectManager.h"
#include "AnimatedGameObject.h"
#include "SoundFX.h"

class Stinger :public AnimatedGameObject
{
private:
	int blink;
	Circle2D collisionShape;
	Circle2D detectionRadius;
	ObjectManager* pObjectManager;
	int health;
	SoundFX* pSoundFX;
public:
	void Initialise(Vector2D initialPos, ObjectManager* pOM, SoundFX* sound);
	void Update(double gt);
	void Render()override;
	IShape2D& GetShape();
	void HandleCollision(GameObject& other);
	IShape2D& GetDetectionRadius();
	void HandleDetection(GameObject& other);
};


