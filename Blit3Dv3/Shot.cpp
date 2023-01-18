#include "Shot.h"
#include "Asteroid.h"
/**
* @return An instance of the Shot class.
*/
Shot::Shot(float newRadius, Sprite* newSprite)
{
	this->radius = newRadius * 10;
	this->radiusOrtho = newRadius / sqrt(2);
	this->sprite = newSprite;
}
/**
* Sets the shot's position.
* @param glm::vec2 The new value of the shot's position.
*/
void Shot::SetPosition(glm::vec2 newPosition)
{
	this->position = newPosition;
}
/**
* Sets the shot's velocity.
* @param glm::vec2 The new value of the shot's velocity.
*/
void Shot::SetVelocity(glm::vec2 newVelocity) {
	this->velocity = newVelocity;
}
/**
* Sets the shot's angle.
* @param glm::vec2 The new value of the shot's angle.
*/
void Shot::SetAngle(float newAngle) {
	this->angle = newAngle;
}
/**
* Draws the shot on the screen.
*/
void Shot::Draw()
{
	this->sprite->angle = this->angle;
	this->sprite->Blit(this->position.x, this->position.y, this->radiusOrtho, this->radiusOrtho);
}
/**
* Updates the shot's values after certain time period.
* @param float The time period that has occurred since last uptade.
* @return Returns true if alive and false if timeToLive <=0.
*/
bool Shot::Update(float seconds)
{
	//TODO:
	// determine if we are past the end of our life time,
	//if so return false
	if (this->timeToLive <= 0) {
		return false;
	}
	//move the shot
	this->position += this->velocity * seconds;
	this->timeToLive -= seconds;
	//TODO:
	//wrap around
	//bounds check position
	if (this->position.x < 0) this->position.x += 1920.f;
	if (this->position.x > 1920) this->position.x -= 1920.f;
	if (this->position.y < 0) this->position.y += 1080.f;
	if (this->position.y > 1080) this->position.y -= 1080.f;

	return true;
}
/**
* This method checks the collition of the shots with the asteroids
* @param std::vector<Shot*> The list of shots
* @param std::vector<Asteroid*> The list of asteroids
* @return the score if the asteroid hit or 0 otherwise
*/
int Shot::CollideWithAsteroids(std::vector<Asteroid*>& asteroids)
{
	int collided = 0;
	if (this->timeToLive > 0)
	{	
		// For each asteroid check if the bullet collided with the asteroid
		for (auto& asteroid : asteroids)
		{
			if (this->CollideWithAsteroid(asteroid))
			{
				collided = asteroid->GetScore();
				asteroid->GotHitByBullet(asteroids);
				this->HitAnAsteroid();
				break;
			}
		}
	}
	return collided;
}
/**
* This method detects the collision of a shot with an asteroid.
* @param Asteroid* The asteroid to detect colission with
* @return True if the shot collided with the asteroid, false otherwise
*/
bool Shot::CollideWithAsteroid(Asteroid* asteroid)
{
	bool collision = false;
	// Check if the bullet and the asteroid could collide
	if (!((asteroid->GetPositionX() + asteroid->GetRadius() < this->position.x - this->radius) ||
		(this->position.x + this->radius < asteroid->GetPositionX() - asteroid->GetRadius()) ||
		(this->position.y + this->radius < asteroid->GetPositionY() - asteroid->GetRadius()) ||
		(asteroid->GetPositionY() + asteroid->GetRadius() < this->position.y - this->radius)))
	{
		// Get the distance and make sure it is less that both of the radius
		float distance = asteroid->Distance(this->position);
		if (distance < (this->radius + asteroid->GetRadius()))
		{
			collision = true;
			asteroid->playExplosionSound();
		}
	}
	return collision;
}
/**
* This method handles the show when it has collided with an asteroid
*/
void Shot::HitAnAsteroid()
{
	this->timeToLive = 0;
}
