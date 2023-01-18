#pragma once

#include "Blit3D.h"
#include "AudioEngine.h"
#include "Shot.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "Asteroid.h"
#include <string>
#include <vector>

#define backgroundWidth 1920
#define backgroundHeight 1080
#define timerWait .5
#define shotSize .2
#define shotVelocity 1000

/**
* This class represents a spaceship and its behaviour.
*/
class Spaceship
{
private:
	/**
	* The sprite that represents the shot graphically.
	*/
	Sprite* shotSprite;
	/**
	* The hield sprite
	*/
	Sprite* shieldSprite;
	/**
	* The sprite array that represents the spaceship graphically.
	*/
	std::vector<Sprite*> spriteList;
	/**
	* The 2D vector that represents the spaceship's direction and speed.
	*/
	glm::vec2 velocity;
	/**
	* The 2D vector that represents the spaceship's position.
	*/
	glm::vec2 position;
	/**
	* A float representing the spaceship's angle.
	*/
	float angle;
	/**
	* A float representing the spaceship's size.
	*/
	float radius;
	/**
	* A float representing the spaceship's size's projection.
	*/
	float radiusOrtho;
	/**
	* The number of the player's lives.
	*/
	int lives = 3;
	/**
	* The number of the player's power-ups.
	*/
	int powerUps = 0;
	/**
	* The number of frame to draw.
	*/
	int frameNumber = 0;
	/**
	* This variable is true if the ship got hit and must put the shield up.
	*/
	bool shieldUp = false;
	/**
	* The shield timer
	*/
	float shieldTimer = 0;
	/**
	* The shield blink animation state
	*/
	bool shieldAnimationState = false;
	/**
	* The shield blink time
	*/
	float shieldAnimationTime = 0;
	/**
	* The timer to end thrusting after the button is pressed.
	*/
	float thrustTimer = 0;
	/**
	* The timer to avoid the user from spamming the shot button.
	*/
	float shotTimer = 0;
	/**
	* Indicates whether the ship is thrusting or not.
	*/
	bool thrusting = false;
	/**
	* Indicates whether the ship is turning left or not.
	*/
	bool turningLeft = false;
	/**
	* Indicates whether the ship is rutning right or not.
	*/
	bool turningRight = false;
	/**
	* Indicates whether the ship exploded.
	*/
	bool exploded = false;
	/**
	* The reference to the main audio object
	*/
	AudioEngine*& audioEngine;
	/** 
	* The ships audio Game object ID
	*/
	AkGameObjectID soundId = 2;
	/**
	* The Game Object sound elements
	*/
	AkPlayingID shieldSound;
	/**
	* The array that contains the vectors for the spaceship's gun's position
	*/
	std::vector<std::vector<float>> guns = { {0.0f,5.0f}, {35.0f, 35.0f}, {35.0f, -30.0f}, {40.0f, 60.0f}, {40.0f, -60.0f} };
public:
	/**
	* Spaceship object constructor method.
	* @param glm::vec2 The spaceship's position
	* @param std::vector<Sprite*> The collection of sprites that are the graphical representation of the spaceship.
	* @param float The spaceship's radius.
	* @param float The spaceship's angle.
	* @param AudioEngine& reference to the main game audio engine object.
	* @return An instance of the Spaceship class.
	*/
	Spaceship(glm::vec2, Sprite*, float, float, AudioEngine*&);
	/**
	* Sets the spaceship's velocity to a given 2D vector.
	* @param glm::vec2 The spaceship's new velocity.
	*/
	void SetVelocity(glm::vec2);
	/**
	* Sets the spaceship's position to a given 2D vector.
	* @param glm::vec2 The spaceship's new position.
	*/
	void SetPosition(glm::vec2);
	/**
	* Gets the spaceship's position.
	* @return The spaceship's position.
	*/
	glm::vec2 GetPosition();
	/**
	* Gets the spaceship's radius.
	* @return The spaceship's radius.
	*/
	float GetRadius();
	/**
	* Sets the spaceship's turn left status.
	* @param bool The new value of the turn left status.
	*/
	void SetTurnLeft(bool);
	/**
	* Sets the spaceship's turn right status.
	* @param bool The new value of the turn right status.
	*/
	void SetTurnRight(bool);
	/**
	* Sets the spaceship's thrusting status.
	* @param bool The new value of the thrusting status.
	*/
	void SetThrusting(bool);
	/**
	* Calculates distance from the center of this spaceship to a position.
	* @param glm::vec2 The position to calculate the distance with this asteroid.
	*/
	float Distance(glm::vec2);
	/**
	* Updates the spaceship's values after certain time period.
	* @param float The time period that has occurred since last uptade.
	* @return Returns true.
	*/
	bool Update(float);
	/**
	* Adds a sprite to the sprite list.
	* @param Sprite* The sprite to add to the list.
	*/
	void AddSprite(Sprite*);
	/** 
	* sets the Ship's shield sprite.
	* @param Sprite* The sprite to set as shield.
	*/
	void SetShieldSprite(Sprite*);
	/**
	* Draws the spaceship on the screen.
	*/
	void Draw();
	/**
	* Shoots a Shot object.
	* @param std::vector<Shot>& The list of available Shot objects.
	* @return Returns true.
	*/
	bool Shoot(std::vector<Shot>& shotList);
	/**
	* This method is called when the ship is hit by an steroid.
	*/
	void GotHit();
	/**
	* This method is called when the ship loses all its lives
	*/
	void KillShip();
	/**
	* This method returns true if the ship collided with an asteroid
	* @return True if the ship collided with at least one asteroid
	*/
	bool CollidedWithAsteroids(std::vector<Asteroid*>&);
	/**
	* This method returns true if the ship collided with a power up
	* @return True if the ship collided with at least one powerup
	*/
	bool CollideWithPowerUps(std::vector<PowerUp*>&);
	/**
	* This method returns true if the ship collided with an specific asteroid
	* @return True if the ship collided with the asteroid
	*/
	bool CollideWithAsteroid(Asteroid*&);
	/**
	* This method returns true if the ship collided with an specific powerUp
	* @return True if the ship collided with the power up
	*/
	bool CollideWithPwerUp(PowerUp*&);
	/**
	* This method returns true if the ship was hit and had no shields up and false otherwise.
	* @return True if the ship was destroyed
	*/
	bool IsDestroyed();
	/**
	* This method returns returns the amount of lives the ship has.
	* @return The amount of lives left
	*/
	int GetLives();
	/**
	* This method checks if the ship exploded already.
	*/
	bool Exploded();
	/**
	* This method set the exploded memeber value.
	*/
	void SetExplosion(bool);
	/**
	* This method activates the spaceship's shield.
	*/
	void ActivateShield();
	/**
	* This method activates the spaceship's grabs a power-up.
	*/
	void GrabPowerUp();
	/**
	* This method returns the amount of power ups the ship has.
	* @return The amount of power-ups the ship has.
	*/
	int GetPowerUp();
	/**
	* This method makes a show given a position and a direction
	* @param glm::vec2 The shot's position relative to the ship's position
	* @param glm::vec2 The shot's velocity relative to the ship's velocity
	*/
	Shot MakeShot(int, float);
	/**
	* This method cleand the object before deletion
	*/
	void willDelete();
	/**
	* This method pauses the ship sounds
	*/
	void Pause();
	/**
	* This method resumes the sound
	*/
	void Resume();
};