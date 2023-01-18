#include "Asteroid.h"
#include <cmath>

extern Blit3D* blit3D;

#define angleVariant 10
#define backgroundWidth 1920.0f
#define backgroundHeight 1080.0f
/**
* Asteroid object constructor method.
* @param Sprite* The graphical representation of the asteroid.
* @param glm::vec2 The asteroid's position
* @param float The asteroid's radius.
* @param float The asteroid's mass.
* qparam float The initial rotation speed.
* @param AudioEngine& reference to the main game audio engine object.
* @return An instance of the asteroid class.
*/
Asteroid::Asteroid(AsteroidType newType, std::vector<std::vector<Sprite*>>& newSpriteList, glm::vec2 newPosition, float rotationSpeed, AudioEngine*& newAudioEngine) : audioEngine(newAudioEngine)
{
	this->type = newType;
	this->spriteList = newSpriteList;
	this->position = newPosition;
	this->state = 0;
	switch (this->type)
	{
	case BIG_ASTEROID:
		this->radius = 80.0f;
		this->radiusOrtho = this->radius / 280.0f / sqrt(2);
		this->mass = 3.0f;
		this->soundEvent = "BigAsteroid";
		break;
	case MEDIUM_ASTEROID:
		this->radius = 60.0f;
		this->radiusOrtho = this->radius / 150.0f / sqrt(2);
		this->mass = 1.0f;
		this->soundEvent = "MediumAsteroid";
		break;
	case SMALL_ASTEROID:
		this->radius = 25.0f;
		this->radiusOrtho = this->radius / 50.0f / sqrt(2);
		this->mass = 0.7f;
		this->soundEvent = "SmallAsteroid";
		break;
	default:
		break;
	}
	this->rotationSpeed = rotationSpeed;
}
/**
* Sets the asteroid's velocity to a given 2D vector.
* @param glm::vec2 The asteroid's new velocity.
*/
void Asteroid::SetVelocity(glm::vec2 newVelocity)
{
	this->velocity = newVelocity;
}
/**
* Sets the asteroid's position to a given 2D vector.
* @param glm::vec2 The asteroid's new position.
*/
void Asteroid::SetPosition(glm::vec2 newPosition)
{
	this->position = newPosition;
}
/**
* This method returns the asteroid's type as int
* @return int the asteroid's type
*/
int Asteroid::GetScore()
{
	switch (this->type)
	{
	case BIG_ASTEROID:
		return 10;
		break;
	case MEDIUM_ASTEROID:
		return 30;
		break;
	case SMALL_ASTEROID:
		return 80;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
/**
* Updates the asteroid's values after certain time period.
* @param float The time period that has occurred since last uptade.
*/
bool Asteroid::Update(float deltaTime)
{
	this->collisionSoundTimer += deltaTime;
	if (this->destroyed) 
	{
		// Asteroid's explosion animation when it has been shot
		this->animationTimer += deltaTime;
		if (this->animationTimer > 0.1f) 
		{
			this->state++;
			this->animationTimer -= 0.1f;
		}
		
		if (this->state >= 8) 
		{
			this->state = 7;
			this->doneExploding = true;
		}
	}
	else {
		// Update the position
		this->position.x += 100 * glm::normalize(this->velocity).x / this->mass * deltaTime;
		this->position.y += 100 * glm::normalize(this->velocity).y / this->mass * deltaTime;

		//bounds check position
		if (this->position.x < 0) this->position.x += backgroundWidth;
		if (this->position.x > backgroundWidth) this->position.x -= backgroundWidth;
		if (this->position.y < 0) this->position.y += backgroundHeight;
		if (this->position.y > backgroundHeight) this->position.y -= backgroundHeight;

		if (this->rotationSpeed > 360)
		{
			this->rotationSpeed -= 360;
		}
		// Update the sprite angle
		this->spriteList[this->type][this->state]->angle += this->rotationSpeed * deltaTime;

		if (this->destroyed) {
			return false;
		}
		else {
			return true;
		}
	}
	
}
/**
* Draws the asteroid on the screen.
*/
void Asteroid::Draw()
{
	//left
	if (this->position.x < this->radius) this->spriteList[this->type][this->state]->Blit(this->position.x + backgroundWidth, this->position.y, this->radiusOrtho, this->radiusOrtho);
	//right
	if (this->position.x > backgroundWidth - this->radius) this->spriteList[this->type][this->state]->Blit(this->position.x - backgroundWidth, this->position.y, this->radiusOrtho, this->radiusOrtho);
	//down
	if (this->position.y < this->radius) this->spriteList[this->type][this->state]->Blit(this->position.x, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	//up
	if (this->position.y > backgroundHeight - this->radius) this->spriteList[this->type][this->state]->Blit(this->position.x, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);

	//copies for 4 diagonal corners
	this->spriteList[this->type][this->state]->Blit(this->position.x + backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->type][this->state]->Blit(this->position.x - backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->type][this->state]->Blit(this->position.x - backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->type][this->state]->Blit(this->position.x + backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->type][this->state]->Blit(position.x, position.y, this->radiusOrtho, this->radiusOrtho);
}
/**
* Calculates collisions with other asteroids.
* @param std::vector<glm::vec2> The list of asteroid to check collision with.
*/
bool Asteroid::CollideWithAsteroids(std::vector<Asteroid*> asteroids)
{
	if (asteroids.size() == 0)
	{
		return false;
	}
	bool collided = false;
	// For each asteroid check the collission with each asteroid
	for (int i = 0; i < asteroids.size() - 1; i++)
	{
		for (int j = i + 1; j < asteroids.size(); j++)
		{
			if (asteroids[i] != asteroids[j]) {
				if (asteroids[i]->CollideAsteroid(asteroids[j]))
				{
					// Collide the asteroid with the other asteroid
					asteroids[i]->Collide(asteroids[j]);
					collided = true;
					asteroids[i]->playCollisionSound();
				}
			}
		}
	}
	return collided;
}
/**
* Calculates distance from the center of this asteroid to a point.
* @param glm::vec2 The point to calculate the distance with this asteroid.
*/
float Asteroid::Distance(glm::vec2 point) {
	float distance = sqrt(pow(this->position.x - point.x, 2.0f) + pow(this->position.y - point.y, 2.0f));
	return distance;
}
/**
* Calculates the asteroid's changes after a detected collision.
* @param Asteroid* The asteroid to which this asteroid collided.
*/
void Asteroid::Collide(Asteroid* ball) {
	// Get the vector that represents their collission direction
	glm::vec2 line = this->position - ball->position;
	glm::vec2 normalVector = glm::normalize(line);
	// Get the distance between asteroids
	float length = glm::length(line);
	// Calculate the mass impact
	glm::vec2 vCenterMass = (this->velocity * this->mass + ball->velocity + ball->mass) / (this->mass + ball->mass);
	// Reduce the mass impact
	this->velocity -= vCenterMass;
	ball->velocity -= vCenterMass;
	// Reflect the direction
	this->velocity = glm::reflect(this->velocity, normalVector);
	ball->velocity = glm::reflect(ball->velocity, normalVector);
	// Incremeent the mass impact
	this->velocity += vCenterMass;
	ball->velocity += vCenterMass;
	// Fix the position in the case they over lap
	float overlap = (this->radius + ball->radius) - length;
	overlap = overlap / 2.f;
	this->position += normalVector + overlap;
	ball->position -= normalVector + overlap;
}
/**
* This method determines if this asteroid has collided with another asteroid.
* @param Asteroid* The asteroid to check collision.
* @return true if they collided and false if they didn't.
*/
bool Asteroid::CollideAsteroid(Asteroid* asteroid) {
	bool collision = false;
	// Check if asteroids could collide
	if (!((asteroid->position.x + asteroid->radius < this->position.x - this->radius) ||
		(this->position.x + this->radius < asteroid->position.x - asteroid->radius) ||
		(this->position.y + this->radius < asteroid->position.y - asteroid->radius) ||
		(asteroid->position.y + asteroid->radius < this->position.y - this->radius)))
	{
		// Calculate the distance and verify it is less than both radiuses
		float distance = this->Distance(asteroid->position);
		if (distance < (this->radius + asteroid->radius))
		{
			collision = true;
		}
	}
	return collision;
}
/**
* This method handles when the asteroid gets hit.
* @param Asteroid* The asteroid list to add new Asteroids.
* @return the value of hitting the asteroid
*/
int Asteroid::GotHitByBullet(std::vector<Asteroid*>& asteroids)
{
	if (!this->destroyed)
	{
		switch (this->type)
		{
		case BIG_ASTEROID:
			for (int i = 0; i < 2; i++)
			{
				// Create two medium asteroids
				Asteroid* asteroid = new Asteroid(MEDIUM_ASTEROID, this->spriteList, { this->position.x, this->position.y }, (-1 ^ i) * 2 * this->rotationSpeed, this->audioEngine);
				glm::vec2 normalVector = glm::normalize(this->velocity);
				asteroid->SetVelocity({ (-2 ^ i) * normalVector.x, (-2 ^ i) * normalVector.y });
				asteroids.push_back(asteroid);
				this->destroyed = true;
				this->state++;
			}
			return this->GetScore();
			break;
		case MEDIUM_ASTEROID:
			for (int i = 0; i < 2; i++)
			{
				// Create two small asteroids
				Asteroid* asteroid = new Asteroid(SMALL_ASTEROID, this->spriteList, { this->position.x, this->position.y }, (-1 ^ i) * 2 * this->rotationSpeed, this->audioEngine);
				asteroid->SetVelocity(this->GetSmallAsteroidDirection(i));
				asteroids.push_back(asteroid);
				this->destroyed = true;
				this->state++;
			}
			return this->GetScore();
			break;
		case SMALL_ASTEROID:
			this->destroyed = true;
			this->state++;
			return this->GetScore();
			break;
		default:
			return 0;

			break;
		}
	}
}
/**
	* This method gets the direction of the small asteroid given the iteration of the ateroid.
	* For the first one, is the velocity
	* For the second one is velocity + 120 degrees
	* For the third one is velocity - 120 degrees
	* @param int The iteration of asteroid
	* @return The velocity vector.
	*/
glm::vec2 Asteroid::GetSmallAsteroidDirection(int iteration)
{
	/*
		cos 120 = -1 / 2
		cos - 120 = -1 / 2
		sin 120 = .886
		sin - 120 = -.886
	*/
	switch (iteration)
	{
	case 0:
		return { 12 * this->velocity.x, 12 * this->velocity.y };
		break;
	case 1:
		return {(-1.0f * this->position.x - 1.663f * this->position.y) * 4, (1.663f * this->position.x - 1.0f * this->position.y) * 4 };
		break;
	default:
		return { (-1.0f * this->position.x + 1.663f * this->position.y) * 4, (-1.663f * this->position.x - 1.0f * this->position.y) * 4 };
		break;
	}
}
/**
* This method returns the condition of the asteroid,
* @return True if lives = 0,false otherwise
*/
bool Asteroid::IsDead()
{
	if (this->doneExploding) {
		return true;
	}
	else {
		return false;
	}
}
/**
* This method returns the position X of the asteroid
* @return int the position.x of the asteroid
*/
float Asteroid::GetPositionX()
{
	return this->position.x;
}
/**
* This method returns the position Y of the asteroid
* @return int the position.y of the asteroid
*/
float Asteroid::GetPositionY()
{
	return this->position.y;
}
/**
* This method returns theasteroid's radius
* @return float the asteroid's radius
*/
float Asteroid::GetRadius()
{
	return this->radius;
}
/**
* This method plays the proper asteroid collission sound
*/
void Asteroid::playExplosionSound()
{
	AKRESULT panningX = AK::SoundEngine::SetRTPCValue(L"PanningX", (AkRtpcValue)(this->position.x), this->soundId);
	this->explosionSound = this->audioEngine->PlayEvent(this->soundEvent, this->soundId);
}
/**
* This method plays the proper asteroid collission sound
*/
void Asteroid::playCollisionSound()
{
	if (this->collisionSoundTimer > 1.0f)
	{
		AKRESULT panningX = AK::SoundEngine::SetRTPCValue(L"PanningX", (AkRtpcValue)(this->position.x), this->soundId);
		this->collisionSound = this->audioEngine->PlayEvent("Collision", this->soundId);
		this->collisionSoundTimer = 0;
	}
	
}
/**
* This method cleand the object before deletion
*/
void Asteroid::willDelete()
{
	if (this->audioEngine != NULL) delete this->audioEngine;
}
