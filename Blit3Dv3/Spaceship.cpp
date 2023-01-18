#include "Spaceship.h"
#include <cmath>

/**
* Spaceship object constructor method.
* @param Sprite* The graphical representation of the spaceship.
* @param glm::vec2 The spaceship's position
* @param float The spaceship's radius.
* @param AudioEngine& reference to the main game audio engine object.
* @return An instance of the Spaceship class.
*/
Spaceship::Spaceship(glm::vec2 newPosition, Sprite* newShotSprite, float newRadius, float newAngle, AudioEngine*& newAudioEngine): audioEngine(newAudioEngine)
{
	this->audioEngine->RegisterGameObject(this->soundId);
	this->frameNumber = 0;
	this->lives = 3;
	this->shotSprite = newShotSprite;
	this->position = newPosition;
	this->velocity = {0,0};
	this->radius = newRadius;
	this->radiusOrtho = newRadius / 420 / sqrt(2);
	this->angle = newAngle;
	this->ActivateShield();
	
}
/**
* Sets the spaceship's velocity to a given 2D vector.
* @param glm::vec2 The spaceship's new velocity.
*/
void Spaceship::SetVelocity(glm::vec2 newVelocity)
{
	this->velocity = newVelocity;
}
/**
* Sets the spaceship's position to a given 2D vector.
* @param glm::vec2 The spaceship's new position.
*/
void Spaceship::SetPosition(glm::vec2 newPosition)
{
	this->position = newPosition;
}
/**
* Gets the spaceship's position.
* @return The spaceship's position.
*/
glm::vec2 Spaceship::GetPosition()
{
	return this->position;
}
/**
* Gets the spaceship's radius.
* @return The spaceship's radius.
*/
float Spaceship::GetRadius()
{
	return this->radius;
}
/**
* Sets the spaceship's turn left status.
* @param bool The new value of the turn left status.
*/
void Spaceship::SetTurnLeft(bool newStatus)
{
	this->turningLeft = newStatus;
}
/**
* Sets the spaceship's turn right status.
* @param bool The new value of the turn right status.
*/
void Spaceship::SetTurnRight(bool newStatus)
{
	this->turningRight = newStatus;
}
/**
* Sets the spaceship's thrusting status.
* @param bool The new value of the thrusting status.
*/
void Spaceship::SetThrusting(bool newStatus) 
{
	this->thrusting = newStatus;
}
/**
* Calculates distance from the center of this spaceship to a position.
* @param glm::vec2 The position to calculate the distance with this spaceship.
*/
float Spaceship::Distance(glm::vec2 otherPosition)
{
	float distance = sqrt(pow(this->position.x - otherPosition.x, 2) + pow(this->position.y - otherPosition.y, 2));
	return distance;
}
/**
* Adds a sprite to the sprite list.
* @param Sprite* The sprite to add to the list.
*/
void Spaceship::AddSprite(Sprite* newSprite)
{
	this->spriteList.push_back(newSprite);
}
/**
* sets the Ship's shield sprite.
* @param Sprite* The sprite to set as shield.
*/
void Spaceship::SetShieldSprite(Sprite* newShieldSprite)
{
	this->shieldSprite = newShieldSprite;
}
/**
* Updates the spaceship's values after certain time period.
* @param float The time period that has occurred since last uptade.
* @return Returns true.
*/
bool Spaceship::Update(float seconds)
{
	if (this->IsDestroyed()) return false;
	//handle shield and make it blink if it is turned on
	if (this->shieldUp) {
		this->shieldTimer += seconds;
		if (this->shieldTimer > this->shieldAnimationTime)
		{
			if (this->shieldAnimationState)
			{
				this->shieldTimer -= this->shieldAnimationTime;
				this->shieldAnimationTime = this->shieldAnimationTime / 2;
			}
			else {
				this->shieldTimer = 0.02f;
			}
			this->shieldAnimationState = !this->shieldAnimationState;
		}
		if (this->shieldAnimationTime < 0.01)
		{
			this->shieldUp = false;
		}
	}
	//handle turning
	if (this->turningLeft)
	{
		this->angle += 180.f * seconds;
	}

	if (this->turningRight)
	{
		this->angle -= 180.f * seconds;
	}
	// Handle thrusting
	if (this->thrusting)
	{
		//calculate facing vector
		float radians = angle * (M_PI / 180);
		glm::vec2 facing;
		facing.x = std::cos(radians);
		facing.y = std::sin(radians);

		facing *= seconds * 400.f;

		this->velocity += facing;

		//check if over max speed
		if (this->velocity.length() > 600.f)
		{
			this->velocity = glm::normalize(this->velocity) * 600.f;
		}

		this->thrustTimer += seconds;

		//animation timing
		if (this->thrustTimer >= 1.f / 20.f)
		{
			//change frames
			this->frameNumber++;
			if (this->frameNumber > 3)
				this->frameNumber = 1;

			this->thrustTimer -= 1.f / 20.f;
		}
	}
	else this->frameNumber = 0;

	//update position
	this->position += this->velocity * seconds;

	//bounds check position
	if (this->position.x < 0) this->position.x += 1920.f;
	if (this->position.x > 1920) this->position.x -= 1920.f;
	if (this->position.y < 0) this->position.y += 1080.f;
	if (this->position.y > 1080) this->position.y -= 1080.f;

	//reduce velocity due to "space friction"
	float scale = 1.f - seconds * 0.5f;
	//if (scale < 0) scale = 0;
	this->velocity *= scale;

	//velocity could potentialy get very small:we should
	//cap it to zero when it gets really close,
	//but that is a more advanced topic that
	//requires techniques like std::isnan()
	float length = glm::length(this->velocity);
	if (std::isnan(length) || length < 0.00001f)
		this->velocity = glm::vec2(0, 0);

	//TODO:
	//handle shot timer
	if (this->shotTimer > 0)
		this->shotTimer -= seconds;
	return true;
}
/**
* Draws the spaceship on the screen.
*/
void Spaceship::Draw()
{
	
	//change ship angle because my graphics face "up", not "right"
	this->spriteList[this->frameNumber]->angle = angle - 90;
	// shield's angle
	this->shieldSprite->angle = angle - 90;
	//draw main ship sprite
	this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y, this->radiusOrtho, this->radiusOrtho);
	if (this->shieldAnimationState)
	{
		this->shieldSprite->Blit(this->position.x, this->position.y, this->radiusOrtho, this->radiusOrtho);
	}
	//redraw if too close to an edge
	//left
	if (this->position.x < this->radius + 10.f) this->spriteList[this->frameNumber]->Blit(this->position.x + 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	if (this->shieldAnimationState && this->position.x < this->radius + 10.f)
	{
		this->shieldSprite->Blit(this->position.x + 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	}
	//right
	if (this->position.x > 1920.f - (this->radius + 10.f)) this->spriteList[this->frameNumber]->Blit(this->position.x - 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	if (this->shieldAnimationState && this->position.x > 1920.f - (this->radius + 10.f))
	{
		this->shieldSprite->Blit(this->position.x - 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	}
	//down
	if (this->position.y < this->radius + 10.f) this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y + 1080.f, this->radiusOrtho, this->radiusOrtho);
	if (this->shieldAnimationState && this->position.y < this->radius + 10.f)
	{
		this->shieldSprite->Blit(this->position.x, this->position.y + 1080.f, this->radiusOrtho, this->radiusOrtho);
	}
	//up
	if (this->position.y > 1080.f - (this->radius + 10.f)) this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y - 1080.f, this->radiusOrtho, this->radiusOrtho);
	if (this->shieldAnimationState && this->position.y > 1080.f - (this->radius + 10.f))
	{
		this->shieldSprite->Blit(this->position.x, this->position.y - 1080.f, this->radiusOrtho, this->radiusOrtho);
	}
	//copies for 4 diagonal corners
	this->spriteList[this->frameNumber]->Blit(this->position.x + backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x - backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x - backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x + backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	if (this->shieldAnimationState)
	{
		this->shieldSprite->Blit(this->position.x + backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
		this->shieldSprite->Blit(this->position.x - backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
		this->shieldSprite->Blit(this->position.x - backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
		this->shieldSprite->Blit(this->position.x + backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	}
	
	
	
}
/**
	* Shoots a Shot object.
	* @param std::vector<Shot>& The list of available Shot objects.
	* @return Returns true.
	*/
bool Spaceship::Shoot(std::vector<Shot>& shotList)
{
	if (this->shotTimer > 0) return false;

	//TODO:
	//time to shoot!
	//reset shot timer
	this->shotTimer = timerWait;

	//add the main shot to the shotList
	shotList.push_back(this->MakeShot(0, 0.0f));

	if (this->powerUps > 0)
	{
		//add the middle shots to the shotList
		shotList.push_back(this->MakeShot(1, 0.05f));
		shotList.push_back(this->MakeShot(2, -0.05f));
	}
	if (this->powerUps > 1) 
	{
		//add the outer shots to the shotList 
		shotList.push_back(this->MakeShot(3, 0.2f));
		shotList.push_back(this->MakeShot(4, -0.2f));
	}
	AKRESULT panningX = AK::SoundEngine::SetRTPCValue(L"PanningX", (AkRtpcValue)(this->position.x), this->soundId);
	AkPlayingID shootSound = this->audioEngine->PlayEvent("Shoot", this->soundId);
	return true;
}
/**
* This method makes a show given a position and a direction
* @param int The shot's position in the array
* @param float The shot's direction relative to the shipt's direction in radians
*/
Shot Spaceship::MakeShot(int gunPosition, float newDirection)
{
	//make a new shot
	Shot newShot = Shot(shotSize, this->shotSprite);
	//set the shot's sprite and position using the ship's variables
	float radians = (this->angle * (M_PI / 180));
	float orthoAngle = ((this->angle + 90) * (M_PI / 180));
	glm::vec2 facing;
	glm::vec2 sideFacing;
	glm::vec2 tmpShotVelocity;
	facing.x = std::cos(radians);
	facing.y = std::sin(radians);
	sideFacing.x = std::cos(orthoAngle);
	sideFacing.y = std::sin(orthoAngle);
	// Set the shot's starting position
	newShot.SetPosition({ this->position.x + (facing.x * this->guns[gunPosition][0]) + (sideFacing.x * this->guns[gunPosition][1]), 
		this->position.y + (facing.y * this->guns[gunPosition][0]) + (sideFacing.y * this->guns[gunPosition][1]) });
	//build a vector from the ship angle
	tmpShotVelocity.x = std::cos(radians + newDirection);
	tmpShotVelocity.y = std::sin(radians + newDirection);
	//scale up the shot velocity
	tmpShotVelocity *= shotVelocity;
	//add the ship velocity
	tmpShotVelocity += this->velocity;
	
	newShot.SetVelocity(tmpShotVelocity);
	newShot.SetAngle(this->angle);

	return newShot;
}
/**
* This method is called when the ship is hit by an steroid.
*/
void Spaceship::GotHit()
{
	if (this->lives <= 0)
	{
		// Kill the ship if it ran out of lives
		this->KillShip();
	}
	else
	{
		// Use a shield and remove power ups
		this->lives--;
		this->powerUps = 0;
		this->ActivateShield();
	}
}
/**
* This method is called when the ship loses all its lives
*/
void Spaceship::KillShip()
{
	this->lives = -1;
}
/**
* This method returns true if the ship was hit and had no shields up and false otherwise.
*/
bool Spaceship::IsDestroyed()
{
	if (this->lives < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/**
* This method returns true if the ship collided with an asteroid
*/
bool Spaceship::CollidedWithAsteroids(std::vector<Asteroid*>& asteroids)
{
	if (this->shieldUp)
	{
		return false;
	}
	bool collided = false;
	// For each asteroid check if the ship collided with it
	for (auto& asteroid : asteroids)
	{
		if (this->CollideWithAsteroid(asteroid))
		{
			if (this->lives > 0)
			{
				asteroid->GotHitByBullet(asteroids);
			}
			this->GotHit();
			collided = true;
			break;
		}
	}
	return collided;
}
/**
	* This method returns true if the ship collided with a power up
	* @return True if the ship collided with at least one powerup
	*/
bool Spaceship::CollideWithPowerUps(std::vector<PowerUp*>& powerUps)
{
	bool collided = false;
	// for each power up, check if the ship has grabbed it
	for (auto& powerUp : powerUps)
	{
		if (this->CollideWithPwerUp(powerUp))
		{
			powerUp->Grabbed();
			this->GrabPowerUp();
			collided = true;
			break;
		}
	}
	return collided;
}
/**
* This method returns true if the ship collided with an specific asteroid
*/
bool Spaceship::CollideWithAsteroid(Asteroid*& asteroid)
{
	bool collision = false;
	// Check if the ship and asteroid could collide
	if (!((asteroid->GetPositionX() + asteroid->GetRadius() < this->position.x - this->radius) ||
		(this->position.x + this->radius < asteroid->GetPositionX() - asteroid->GetRadius()) ||
		(this->position.y + this->radius < asteroid->GetPositionY() - asteroid->GetRadius()) ||
		(asteroid->GetPositionY() + asteroid->GetRadius() < this->position.y - this->radius)))
	{
		// Calculate the distance and check it is less than both radius
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
* This method returns true if the ship collided with an specific powerUp
* @return True if the ship collided with the power up
*/
bool Spaceship::CollideWithPwerUp(PowerUp*& powerUp)
{
	bool collision = false;
	// Check if the ship and power up could collide
	if (!((powerUp->GetPositionX() + powerUp->GetRadius() < this->position.x - this->radius) ||
		(this->position.x + this->radius < powerUp->GetPositionX() - powerUp->GetRadius()) ||
		(this->position.y + this->radius < powerUp->GetPositionY() - powerUp->GetRadius()) ||
		(powerUp->GetPositionY() + powerUp->GetRadius() < this->position.y - this->radius)))
	{
		// Calculate the distance and check it is less than both radius
		float distance = powerUp->Distance(this->position);
		if (distance < (this->radius + powerUp->GetRadius()))
		{
			collision = true;
		}
	}
	return collision;
}
/**
* This method returns returns the amount of lives the ship has.
* @return The amount of lives left
*/
int Spaceship::GetLives()
{
	return this->lives;
}
/**
* This method checks if the ship exploded already.
*/
bool Spaceship::Exploded()
{
	return this->exploded;
}
/**
* This method set the exploded memeber value.
*/
void Spaceship::SetExplosion(bool explosionValue)
{
	this->exploded = explosionValue;
}
/**
* This method activates the spaceship's shield.
*/
void Spaceship::ActivateShield()
{
	this->shieldUp = true;
	this->shieldTimer = 0;
	this->shieldAnimationState = true;
	this->shieldAnimationTime = 1.0f;
	AKRESULT panningX = AK::SoundEngine::SetRTPCValue(L"PanningX", (AkRtpcValue)(this->position.x), this->soundId);
	this->shieldSound = this->audioEngine->PlayEvent("Shield", this->soundId);

}
/**
* This method activates the spaceship's grabs a power-up.
*/
void Spaceship::GrabPowerUp()
{
	if (this->powerUps < 2) 
	{
		this->powerUps++;
	}
}
/**
* This method returns the amount of power ups the ship has.
* @return The amount of power-ups the ship has.
*/
int Spaceship::GetPowerUp()
{
	return this->powerUps;
}
/**
* This method cleand the object before deletion
*/
void Spaceship::willDelete()
{
	//if (this->audioEngine != NULL) delete this->audioEngine;
}
/**
* This method pauses the ship sounds
*/
void Spaceship::Pause()
{
	this->audioEngine->PauseEvent("Shield", this->soundId, this->shieldSound);
}
/**
* This method resumes the sound
*/
void Spaceship::Resume()
{
	this->audioEngine->ResumeEvent("Shield", this->soundId, this->shieldSound);
}