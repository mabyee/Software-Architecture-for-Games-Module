/*
Created by Henri Puennel, W19019002.
This file...
*/
#pragma once
#include "ObjectManager.h"
#include "SoundFX.h"
#include "score.h"
#include "AnimatedGameObject.h"

class Soldier:public AnimatedGameObject
{
private:
	//animation
	int walk, idle;
	//health bar colours and shapes
	unsigned int colourRed, colourGreen;
	Rectangle2D HealthBar, DamageBar;

	//soldier attributes
	int stingerAmmo, ammo;
	int health;
	int cardCount;
	int accessCount;

	//movement stats
	Vector2D velocity;
	Vector2D acceleration;
	Vector2D friction;

	//sound
	SoundFX* pSoundFX;

	//other
	float timer;
	float gameTime;
	ObjectManager* pObjectManager; // Pointer to objectmanager required when an object creates another object
	Circle2D collisionShape;
	Circle2D detectionRadius;
	Score* pScore;
public:
	void Initialise(Vector2D initialPos, ObjectManager* pOM, SoundFX* sound, Score* pScore);
	void Update(double gt);
	void Render() override;
	IShape2D& GetShape();
	IShape2D& GetDetectionRadius();
	void HandleCollision(GameObject& other);
	void HandleDetection(GameObject& other);
	int GetAmmo();
	int GetStingerAmmo();
	int GetHealth();
	int GetCardCount();
	int GetAccessCount();
	void DealDamage(int damageAmount);
};