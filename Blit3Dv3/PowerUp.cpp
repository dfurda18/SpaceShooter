#include "PowerUp.h"
/**
* @param glm::vec2 The power-up's position
* @param float The power-up's radius
* @param Sprite* The power-up's sprite
* @return An instance of the power-up class.
*/
PowerUp::PowerUp(glm::vec2 newPosition, float newRadius, Sprite* newSprite)
{
	this->radius = newRadius;
	this->radiusOrtho = newRadius / 50 / sqrt(2);
	this->sprite = newSprite;
	this->position = newPosition;
	this->grabbed = false;
}
/**
* Draws the power-up on the screen.
*/
void PowerUp::Draw()
{
	this->sprite->Blit(this->position.x, this->position.y, this->radiusOrtho, this->radiusOrtho);
}
/**
* Updates the power-up's values after certain time period.
* @param float The time period that has occurred since last uptade.
* @return Returns true if alive and false if timeToLive <=0.
*/
bool PowerUp::Update(float seconds)
{
	if (this->grabbed)
	{
		return false;
	}
	return true;
}
/**
* This method is called when a power up is grabbed
*/
bool PowerUp::Grabbed()
{
	this->grabbed = true;
	return this->grabbed;
}
/**
* This method returns the position X of the power up
* @return float the position.x of the power up
*/
float PowerUp::GetPositionX()
{
	return this->position.x;
}
/**
* This method returns the position Y of the power up
* @return float the position.y of the power up
*/
float PowerUp::GetPositionY()
{
	return this->position.y;
}
/**
* This method returns the power up's radius
* @return float the power up's radius
*/
float PowerUp::GetRadius()
{
	return this->radius;
}
/**
* Calculates distance from the center of this power up to a position.
* @param glm::vec2 The position to calculate the distance with this power up.
*/
float PowerUp::Distance(glm::vec2 point)
{
	float distance = sqrt(pow(this->position.x - point.x, 2.0f) + pow(this->position.y - point.y, 2.0f));
	return distance;
}