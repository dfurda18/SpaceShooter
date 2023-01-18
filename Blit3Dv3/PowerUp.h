#pragma once

#include<Blit3D.h>

/**
* This class represents a power-up and its behaviour.
*/
class PowerUp
{
private:
	/**
	* The position of the power-up.
	*/
	glm::vec2 position;
	/**
	* The sprite representing the power-up.
	*/
	Sprite* sprite = NULL;
	/**
	* A float representing the power-up's size.
	*/
	float radius;
	/**
	* A float representing the power-up's size's projection.
	*/
	float radiusOrtho;
	/**
	* Boolean that sets the grabbing condition of the power-up
	*/
	bool grabbed;
public:
	/**
	* @param glm::vec2 The power-up's position
	* @param float The power-up's radius
	* @param Sprite* The power-up's sprite
	* @return An instance of the power-up class.
	*/
	PowerUp(glm::vec2, float, Sprite*);
	/**
	* Draws the power-up on the screen.
	*/
	void Draw();
	/**
	* Updates the power-up's values after certain time period.
	* @param float The time period that has occurred since last uptade.
	* @return Returns true if alive and false if timeToLive <=0.
	*/
	bool Update(float seconds);
	/**
	* This method is called when a power up is grabbed
	*/
	bool Grabbed();
	/**
	* This method returns the position X of the power up
	* @return float the position.x of the power up
	*/
	float GetPositionX();
	/**
	* This method returns the position Y of the power up
	* @return float the position.y of the power up
	*/
	float GetPositionY();
	/**
	* This method returns the power up's radius
	* @return float the power up's radius
	*/
	float GetRadius();
	/**
	* Calculates distance from the center of this power up to a position.
	* @param glm::vec2 The position to calculate the distance with this power up.
	*/
	float Distance(glm::vec2);
};