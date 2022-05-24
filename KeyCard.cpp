/*
Created by Henri Puennel, W19019002.
This file...
*/
#include "KeyCard.h"
#include "soldier.h"

void KeyCard::Initialise(Vector2D pos)
{
	position.set(pos);
	LoadImg(L"Images/KeyCard/keyCard.png");
	angle = 0;
	active = true;
	scale = 1.0f;
}
void KeyCard::Update(double gt)
{

}

IShape2D& KeyCard::GetShape()
{
	collisionShape.PlaceAt(position, 8);
	return collisionShape;
}
IShape2D& KeyCard::GetDetectionRadius()
{
	detectionRadius.PlaceAt(position, 1);
	return detectionRadius;
}

void KeyCard::HandleCollision(GameObject& other)
{
	if (typeid(other) == typeid(Soldier))
	{
		Deactivate();
	}
}
void KeyCard::HandleDetection(GameObject& other)
{

}
