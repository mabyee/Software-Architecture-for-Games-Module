#pragma once
#include "GameObject.h"
#include "ObjectManager.h"
#include "SoundFX.h"
#include "score.h"

class Soldier:public GameObject
{
private:
	// soldier walking animation
	float animationSpeed;
	float currentImage;
	PictureIndex soldierImages[9];

	//health bar colours and shapes
	unsigned int colourRed;
	unsigned int colourGreen;
	Rectangle2D HealthBar;
	Rectangle2D DamageBar;

	//soldier attributes
	int stingerAmmo;
	int ammo;
	int health;

	//movement stats
	Vector2D velocity;
	Vector2D acceleration;
	Vector2D friction;

	//sound
	SoundIndex shootSound;
	SoundIndex thrustLoop;
	SoundFX* pSoundFX;

	//other
	ObjectManager* pObjectManager; // Pointer to objectmanager required when an object creates another object
	Circle2D collisionShape;
	Score* pScore;
public:
	void Initialise(Vector2D initialPos, ObjectManager* pOM, SoundFX* sound, Score* pScore);
	void Update(double gt);
	void Render() override;
	IShape2D& GetShape();
	void HandleCollision(GameObject& other);
};