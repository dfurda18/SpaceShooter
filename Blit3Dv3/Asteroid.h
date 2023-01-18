#pragma once

#include "Blit3D.h"
#include "AudioEngine.h"
#include <string>
#include <random>
#include <vector>

enum AsteroidType {BIG_ASTEROID = 0, MEDIUM_ASTEROID = 1, SMALL_ASTEROID = 2};
/**
* This class represents an asteroid and its behaviour.
*/
class Asteroid
{
private:
	/**
	* Attribute tha tindicates if the asteroid is destroyed.
	*/
	bool destroyed = false;
	/**
	* Attribute tha tindicates if theexplosion animation is over.
	*/
	bool doneExploding = false;
	/**
	* The asteroid's animation state.
	*/
	int state;
	/**
	* The type of asteroid
	*/
	AsteroidType type;
	/**
	* The sprites that represents the asteroids graphucally.
	*/
	std::vector<std::vector<Sprite*>> spriteList;
	/**
	* The 2D vector that represents the asteroid's direction and speed.
	*/
	glm::vec2 velocity;
	/**
	* The 2D vector that represents the asteroid's position.
	*/
	glm::vec2 position;
	/**
	* A float representing the asteroid's size.
	*/
	float radius;
	/**
	* Animation timer.
	*/
	float animationTimer = 0;
	/**
	* A float representing the asteroid's size's projection.
	*/
	float radiusOrtho;
	/**
	* A float representing the asteroid's mass.
	*/
	float mass;
	/**
	* A float representing the asteroid's rotation speed and direction.
	*/
	float rotationSpeed;
	/**
	* The reference to the main audio object
	*/
	AudioEngine*& audioEngine;
	/**
	* The ships audio Game object ID
	*/
	AkGameObjectID soundId = 4;
	/**
	* The ships audio Game object ID
	*/
	std::string soundEvent;
	/**
	* The Game Object sound elements
	*/
	AkPlayingID collisionSound, explosionSound;
	/**
	* The collission timer
	*/
	float collisionSoundTimer = 2;

public:
	/**
	* Asteroid object constructor method.
	* @param Sprite* The graphical representation of the asteroid.
	* @param glm::vec2 The asteroid's position
	* @param float The asteroid's radius.
	* @param float The asteroid's mass.
	* @param float The initial rotation speed.
	* @param AudioEngine& reference to the main game audio engine object.
	* @return An instance of the asteroid class.
	*/
	Asteroid(AsteroidType, std::vector<std::vector<Sprite*>>&, glm::vec2, float, AudioEngine*&);
	/**
	* Sets the asteroid's velocity to a given 2D vector.
	* @param glm::vec2 The asteroid's new velocity.
	*/
	void SetVelocity(glm::vec2);
	/**
	* Sets the asteroid's position to a given 2D vector.
	* @param glm::vec2 The asteroid's new position.
	*/
	void SetPosition(glm::vec2);
	/**
	* This method returns the position X of the asteroid
	* @return float the position.x of the asteroid
	*/
	float GetPositionX();
	/**
	* This method returns the position Y of the asteroid
	* @return float the position.y of the asteroid
	*/
	float GetPositionY();
	/**
	* This method returns theasteroid's radius
	* @return float the asteroid's radius
	*/
	float GetRadius();
	/**
	* This method returns the asteroid's score
	* @return int the asteroid's score
	*/
	int GetScore();
	/**
	* Updates the asteroid's values after certain time period.
	* @param float The time period that has occurred since last uptade.
	*/
	bool Update(float);
	/**
	* Draws the asteroid on the screen.
	*/
	void Draw();
	/**
	* Calculates collisions with other asteroids.
	* @param std::vector<glm::vec2> The list of asteroid to check collision with.
	*/
	static bool CollideWithAsteroids(std::vector<Asteroid*>);
	/**
	* Calculates distance from the center of this asteroid to a point.
	* @param glm::vec2 The point to calculate the distance with this asteroid.
	*/
	float Distance(glm::vec2);
	/**
	* Calculates the asteroid's changes after a detected collision.
	* @param Asteroid* The asteroid to which this asteroid collided.
	*/
	void Collide(Asteroid*);
	/**
	* This method determines if this asteroid has collided with another asteroid.
	* @param Asteroid* The asteroid to check collision.
	* @return true if they collided and false if they didn't.
	*/
	bool CollideAsteroid(Asteroid*);
	/**
	* This method handles when the asteroid gets hit.
	* @param Asteroid* The asteroid list to add new Asteroids.
	*/
	int GotHitByBullet(std::vector<Asteroid*>&);
	/**
	* This method gets the direction of the small asteroid given the iteration of the ateroid.
	* For the first one, is the velocity
	* For the second one is velocity + 120 degrees
	* For the third one is velocity - 120 degrees
	* @param int The iteration of asteroid
	* @return The velocity vector.
	*/
	glm::vec2 GetSmallAsteroidDirection(int);
	/**
	* This method returns the condition of the asteroid,
	* @return True if lives = 0,false otherwise
	*/
	bool IsDead();
	/**
	* This method plays the proper asteroid collission sound
	*/
	void playExplosionSound();
	/**
	* This method cleand the object before deletion
	*/
	void willDelete();
	/**
	* This method cleand the object before deletion
	*/
	void playCollisionSound();
};