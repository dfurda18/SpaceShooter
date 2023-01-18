#pragma once

#include<Blit3D.h>
#include "Asteroid.h"

/**
* This class represents a shot and its behaviour.
*/
class Shot
{
private:
	/**
	* The velocity of the shot.
	*/
	glm::vec2 velocity;
	/**
	* The position of the shot.
	*/
	glm::vec2 position;
	/**
	* A float representing the shot's angle.
	*/
	float angle = 0;
	/**
	* The sprite representing the shot.
	*/
	Sprite* sprite = NULL;
	/**
	* The time the shot has to live.
	*/
	float timeToLive = 1.0f; //shots live for 1.5 seconds
	/**
	* A float representing the shot's size.
	*/
	float radius;
	/**
	* A float representing the shot's size's projection.
	*/
	float radiusOrtho;
public:
	/**
	* @param float The shot's radius
	* @param Sprite* The shot's sprite
	* @return An instance of the Shot class.
	*/
	Shot(float, Sprite*);
	/**
	* Sets the shot's position.
	* @param glm::vec2 The new value of the shot's position.
	*/
	void SetPosition(glm::vec2);
	/**
	* Sets the shot's velocity.
	* @param glm::vec2 The new value of the shot's velocity.
	*/
	void SetVelocity(glm::vec2);
	/**
	* Sets the shot's angle.
	* @param glm::vec2 The new value of the shot's angle.
	*/
	void SetAngle(float);
	/**
	* Draws the shot on the screen.
	*/
	void Draw();
	/**
	* Updates the shot's values after certain time period.
	* @param float The time period that has occurred since last uptade.
	* @return Returns true if alive and false if timeToLive <=0.
	*/
	bool Update(float seconds);
	/**
	* This method checks the collition of the shots with the asteroids
	* @param std::vector<Shot*> The list of shots
	* @param std::vector<Asteroid*> The list of asteroids
	*/
	int CollideWithAsteroids(std::vector<Asteroid*>&);
	/**
	* This method detects the collision of a shot with an asteroid.
	* @param Asteroid* The asteroid to detect colission with
	* @return True if the shot collided with the asteroid, false otherwise
	*/
	bool CollideWithAsteroid(Asteroid*);
	/**
	* This method handles the show when it has collided with an asteroid
	*/
	void HitAnAsteroid();
};