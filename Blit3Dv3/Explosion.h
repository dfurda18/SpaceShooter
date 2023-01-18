#pragma once

#include "Blit3D.h"
#include "AudioEngine.h"
#include <string>
#include <vector>

#define backgroundWidth 1920
#define backgroundHeight 1080

/**
* This class represents a spaceship and its behaviour.
*/
class Explosion
{
private:

	/**
	* The sprite array that represents the splosion graphically.
	*/
	std::vector<Sprite*> spriteList;
	/**
	* The number of frame to draw.
	*/
	int frameNumber = 0;
	/**
	* The 2D vector that represents the explosion's position.
	*/
	glm::vec2 position;
	/**
	* A float representing the explosion's size's radius.
	*/
	float radius;
	/**
	* A float representing the explosion's size's radius projection.
	*/
	float radiusOrtho;
	/**
	* Animation timer.
	*/
	float animationTimer = 0;
public:
	/**
	* Explosion object constructor method.
	* @param glm::vec2 The explosion's position
	* @param std::vector<Sprite*> The collection of sprites that are the graphical representation of the explosion.
	* @param float The explosion's radius size.
	* @param AudioEngine& reference to the main game audio engine object.
	* @return An instance of the Explosion class.
	*/
	Explosion(glm::vec2, std::vector<Sprite*>, float);
	/**
	* This method returns the explosion's current frame
	* @return int the explosion's current frame
	*/
	int GetFrame();
	/**
	* Updates the explosion's values after certain time period.
	* @param float The time period that has occurred since last uptade.
	* @return Returns true.
	*/
	bool Update(float);
	/**
	* Draws the exlosion on the screen.
	*/
	void Draw();
};
