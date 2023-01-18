#include "Explosion.h"

/**
* Explosion object constructor method.
* @param glm::vec2 The explosion's position
* @param std::vector<Sprite*> The collection of sprites that are the graphical representation of the explosion.
* @param float The explosion's radius size.
* @param AudioEngine& reference to the main game audio engine object.
* @return An instance of the Explosion class.
*/
Explosion::Explosion(glm::vec2 newPosition, std::vector<Sprite*> newSpriteList, float newRadius)
{
	this->frameNumber = 0;
	this->position = newPosition;
	this->spriteList = newSpriteList;
	this->radius = newRadius * 1.2f;
	this->radiusOrtho = this->radius / 220 / sqrt(2);
}
/**
* This method returns the explosion's current frame
* @return int the explosion's current frame
*/
int Explosion::GetFrame()
{
	return this->frameNumber;
}
/**
* Updates the explosion's values after certain time period.
* @param float The time period that has occurred since last uptade.
* @return Returns true.
*/
bool Explosion::Update(float deltaTime)
{
	// Progress the animation if certain amount of time has passed.
	this->animationTimer += deltaTime;
	if (this->animationTimer > 0.1f) {
		this->frameNumber++;
		this->animationTimer -= 0.1f;
	}

	if (this->frameNumber >= 10) {
		this->frameNumber = 10;
	}
	return true;
}
/**
* Draws the exlosion on the screen.
*/
void Explosion::Draw()
{
	//change explosion angle because my graphics face "up", not "right"
	this->spriteList[this->frameNumber]->angle = - 90;

	//draw main ship sprite
	this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y, this->radiusOrtho, this->radiusOrtho);

	//redraw if too close to an edge
	//left
	if (this->position.x < this->radius + 10.f) this->spriteList[this->frameNumber]->Blit(this->position.x + 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	//right
	if (this->position.x > 1920.f - (this->radius + 10.f)) this->spriteList[this->frameNumber]->Blit(this->position.x - 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	//down
	if (this->position.y < this->radius + 10.f) this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y + 1080.f, this->radiusOrtho, this->radiusOrtho);
	//up
	if (this->position.y > 1080.f - (this->radius + 10.f)) this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y - 1080.f, this->radiusOrtho, this->radiusOrtho);

	//copies for 4 diagonal corners
	this->spriteList[this->frameNumber]->Blit(this->position.x + backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x - backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x - backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x + backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
}