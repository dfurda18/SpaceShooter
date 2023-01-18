/**
	Asteroids Game main file
*/

//memory leak detection not used
#define CRTDBG_MAP_ALLOC
//Can't do the following if using Wwise in debug mode
/*
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG
*/
#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"
#include "AudioEngine.h"
#include "Shot.h"
#include "PowerUp.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "RandomGenerator.h"

#define backgroundWidth 1920
#define backgroundHeight 1080
#define POWER_UP_SIZE 25

//GLOBAL DATA
enum GameState { TITLE_PAGE = 0, GAME = 1, PAUSE = 2 };
// External resources
Blit3D* blit3D = NULL;
RandomGenerator random;
// Game Objects
Spaceship* ship = NULL;
Explosion* shipExplosion = NULL;
PowerUp* powerUp;
// Game Object's lists
std::vector<Shot> shotList;
std::vector<PowerUp*> powerUpList;
std::vector<Asteroid*> asteroids;
// Sprites
Sprite* backgroundSprite = NULL;
Sprite* shieldIconSprite = NULL;
Sprite* powerUpSprite = NULL;
Sprite* shotInterfaceSprite = NULL;
// Sprite lists
std::vector<std::vector<Sprite*>> spriteLists;
std::vector<Sprite*> bigAsteroidSprites;
std::vector<Sprite*> mediumAsteroidSprites;
std::vector<Sprite*> smallAsteroidSprites;
std::vector<Sprite*> explosionSpriteList;
// Fonts
AngelcodeFont* electroliteFont = NULL;
AngelcodeFont* syneMonoFont = NULL;

// Game states
GameState gameState = TITLE_PAGE;
bool gameOver = false;
bool shoot = false;
// Game variables
int level = 0;
int score = 0;
int lastPowerUp = 0;
bool thrustPressed;
// Game Timers
double elapsedTime = 0;
float timeSlice = 1.f / 120.f;
float levelTitleTimer = 0;
bool notPlayedExplosion = true;
// Audio
AudioEngine * audioE = NULL;
AkGameObjectID mainGameID = 1;
AkGameObjectID asteroidID = 4;
AkPlayingID titleMusicId, gameMusicId, thrustSound, pauseSound, explosionSound;

/**
* This method creates a random vector inside the screen.
* @param int The screen's width.
* @param in The screen's height.
* @return A vector within the screen.
*/
glm::vec2 GetRandomPosition(int width, int height)
{
	glm::vec2 newPosition = { random.RandomFloat(0, width, 1000), random.RandomFloat(0, height, 1000) };
	return newPosition;
}

/**
* This method initialices the scene.
*/
void Init()
{
	gameState = TITLE_PAGE;
	//turn cursor off
	blit3D->ShowCursor(false);
	gameOver = false;
	//load Sprites for background, shield icon, shot icon and power up
	backgroundSprite = blit3D->MakeSprite(0, 0, backgroundWidth, backgroundHeight, "Media\\background.png");
	shieldIconSprite = blit3D->MakeSprite(0, 0, 202, 200, "Media\\shieldIcon.png");
	shotInterfaceSprite = blit3D->MakeSprite(0, 0, 100, 100, "Media\\shotInterface.png");
	powerUpSprite = blit3D->MakeSprite(0, 0, 100, 100, "Media\\shot.png");
	// load all the explosion sprites
	for (int i = 0; i < 10; i++)
	{
		explosionSpriteList.push_back(blit3D->MakeSprite(0 + (i * 1066), 0, 1066, 1091, "Media\\Ship_Exploding.png"));
	}
	// Create the asteroids sprites
	for (int i = 0; i < 8; i++)
	{
		bigAsteroidSprites.push_back(blit3D->MakeSprite(i * 804, 0, 804, 798, "Media\\BigAsteroidSet.png"));
	}
	spriteLists.push_back(bigAsteroidSprites);
	for (int i = 0; i < 8; i++)
	{
		mediumAsteroidSprites.push_back(blit3D->MakeSprite(i * 405, 0, 405, 372, "Media\\MediumAsteroidSet.png"));
	}
	spriteLists.push_back(mediumAsteroidSprites);
	for (int i = 0; i < 8; i++)
	{
		smallAsteroidSprites.push_back(blit3D->MakeSprite(i * 193, 0, 193, 183, "Media\\SmallAsteroidSet.png"));
	}
	spriteLists.push_back(smallAsteroidSprites);

	// Font
	electroliteFont = blit3D->MakeAngelcodeFontFromBinary32("Media\\fonts\\electrolite.bin");
	syneMonoFont = blit3D->MakeAngelcodeFontFromBinary32("Media\\fonts\\SyneMono.bin");
	//set the clear colour
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);	//clear colour: r,g,b,a 
	
	//create audio engine
	audioE = new AudioEngine;
	audioE->Init();
	audioE->SetBasePath("Media\\Music\\");

	//load banks
	audioE->LoadBank("Init.bnk");
	audioE->LoadBank("Main_Sound.bnk");

	//register our game objects
	audioE->RegisterGameObject(mainGameID);
	audioE->RegisterGameObject(asteroidID);

	//start playing the looping drums
	//We can play events by name:
	titleMusicId = audioE->PlayEvent("TitleMusic", mainGameID);
}

/**
* This method resotre's the scene initialization.
*/
void DeInit(void)
{
	
	// Eliminate ship, asteroids and power ups
	//ship->willDelete();
	if (ship != NULL) delete ship;
	for (auto asteroid : asteroids)
	{
		//asteroid->willDelete();
		if (asteroid != NULL) delete asteroid;
	}
	asteroids.clear();
	for (auto powerUp : powerUpList)
	{
		if (powerUp != NULL) delete powerUp;
	}
	powerUpList.clear();
	if (audioE != NULL) delete audioE;
}

/**
* This method is called every update.
*/
void Update(double seconds)
{
	//must always update audio in our game loop
	audioE->ProcessAudio();

	switch (gameState)
	{
	case TITLE_PAGE:
		break;
	case GAME:
		// Check if you ran out of asteroids and reloas a level if you do
		if (asteroids.size() <= 0) {
			level++;
			for (int i = 0; i < level; i++)
			{
				Asteroid* asteroid = new Asteroid(BIG_ASTEROID, spriteLists, GetRandomPosition(backgroundWidth, backgroundHeight), random.RandomFloat(1, 10, 1), audioE);
				asteroid->SetVelocity({ random.RandomFloat(-50, 50, 1),random.RandomFloat(-50, 50, 1) });
				asteroids.push_back(asteroid);
			}
			levelTitleTimer = 0;
			ship->ActivateShield();


		}
		//only update time to a maximun amount - prevents big jumps in 
		//the simulation if the computer "hiccups"
		if (seconds < 0.15)
			elapsedTime += seconds;
		else elapsedTime += 0.15;

		//update by a full timeslice when it's time
		while (elapsedTime >= timeSlice)
		{
			elapsedTime -= timeSlice;
			levelTitleTimer += timeSlice;

			

			if (levelTitleTimer > 5) levelTitleTimer = 5;
			// handle impact
			if(!ship->IsDestroyed())
				ship->CollidedWithAsteroids(asteroids);
			// Update ship
			ship->Update(timeSlice);

			// Shoot if shooting
			if (shoot && !ship->IsDestroyed())
			{
				if (ship->Shoot(shotList))
				{
					
				}
			}

			//iterate backwards through the shotlist,
			//so we can erase shots without messing up the vector
			//for the next loop
			for (int i = shotList.size() - 1; i >= 0; --i)
			{
				//shot Update() returns false when the bullet should be killed off
				if (!shotList[i].Update(timeSlice))
				{
					shotList.erase(shotList.begin() + i);
				}
				else
				{
					// Add on to the score with the collided asteroid
					score += shotList[i].CollideWithAsteroids(asteroids);
				}


			}

			// update the asteroids
			for (int i = asteroids.size() - 1; i >= 0; --i)
			{
				// If the asteroids animation has ended, destroy it, the asteroid's explosion can kill you too
				if (asteroids[i]->IsDead())
				{
					delete asteroids[i];
					asteroids.erase(asteroids.begin() + i);
				}
				else
				{
					// Update the asteroid
					asteroids[i]->Update(timeSlice);
				}
			}
			// Check asteroid's collisions with otehr asteroids
			Asteroid::CollideWithAsteroids(asteroids);
			// Handle the ship destruction
			if (ship->IsDestroyed())
			{
				audioE->StopEvent("Thrust", mainGameID, thrustSound);
				// show explosion
				if (shipExplosion == NULL && !ship->Exploded() && notPlayedExplosion)
				{
					notPlayedExplosion = false;
					ship->SetExplosion(true);
					shipExplosion = new Explosion(ship->GetPosition(), explosionSpriteList, ship->GetRadius());
					AKRESULT panningX = AK::SoundEngine::SetRTPCValue(L"PanningX", (AkRtpcValue)(ship->GetPosition().x), mainGameID);
					audioE->ProcessAudio();
					explosionSound = audioE->PlayEvent("Explosion", mainGameID);
					audioE->ProcessAudio();
				}
			}
			// Chandle the explosion state
			if (shipExplosion != NULL)
			{
				//Set game over if the epxlosion animation has ended
				if (shipExplosion->GetFrame() >= 9) {
					gameOver = true;
				}
				// Animate the explosion
				if (shipExplosion->GetFrame() < 10)
				{
					shipExplosion->Update(timeSlice);
				}
				else {
					// Destroy the explosion
					if (shipExplosion != NULL) delete shipExplosion;
					shipExplosion = NULL;
				}
			}
			// Appear power ups if the score has gone over 500 since last power up
			if (ship->GetPowerUp() + powerUpList.size() < 2 && score - lastPowerUp > 500)
			{
				powerUp = new PowerUp(GetRandomPosition(backgroundWidth, backgroundHeight), POWER_UP_SIZE, powerUpSprite);
				powerUpList.push_back(powerUp);
				lastPowerUp = score;
			}
			// Check if the ship grabs a power up
			ship->CollideWithPowerUps(powerUpList);
			// Delete power ups that have been grabbed
			for (int i = powerUpList.size() - 1; i >= 0; --i)
			{
				if (!powerUpList[i]->Update(timeSlice))
				{
					if (powerUpList[i] != NULL) delete powerUpList[i];
					powerUpList.erase(powerUpList.begin() + i);
				}
			}
		}
		break;
	case PAUSE:
		break;
	default:
		break;
	}
	
}

/**
* This method is called to draw all elements.
*/
void Draw(void)
{
	// Variables for the texts
	std::string text;
	float textWidth;
	float textHeight;
	float vMargin;
	float hMArgin;
	switch (gameState)
	{
	case TITLE_PAGE:
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw stuff here

		//draw the background in the middle of the screen
		backgroundSprite->Blit(1920.f / 2, 1080.f / 2);
		// Draw the title page texts
		text = "Space Shooter";
		textWidth = electroliteFont->WidthText(text);
		textHeight = 76.f;
		electroliteFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + textHeight, text);
		text = "Press ENTER to start.";
		textWidth = syneMonoFont->WidthText(text);
		textHeight = 0.f;
		syneMonoFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + textHeight, text);
		text = "Copyright Dario Urdapilleta.";
		textWidth = syneMonoFont->WidthText(text);
		textHeight = 92.f;
		syneMonoFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, textHeight, text);
		break;
	case GAME:
	case PAUSE:
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw the background in the middle of the screen
		backgroundSprite->Blit(1920.f / 2, 1080.f / 2);

		//draw the ship, dissapear it in the explosion's frame 5
		if (shipExplosion != NULL)
		{
			if (shipExplosion->GetFrame() < 5 && shipExplosion->GetFrame() >= 0)
			{
				ship->Draw();
			}
			else {

			}
		}
		else
		{
			if (!ship->IsDestroyed())
			{
				ship->Draw();
			}

		}

		//draw the shots
		for (auto& shot : shotList)
			shot.Draw();
		// Draw the power ups
		for (auto powerUp : powerUpList)
		{
			powerUp->Draw();
		}
		// Draw the asteroids
		for each (Asteroid * asteroid in asteroids)
		{
			asteroid->Draw();
		}
		// Draw the ship's explosion
		if (shipExplosion != NULL)
		{
			if (shipExplosion->GetFrame() < 10 && shipExplosion->GetFrame() >= 0)
			{
				shipExplosion->Draw();
			}
		}
		// Draw texts
		vMargin = 20.0f;
		hMArgin = 30.0f;

		if (gameOver) {
			text = "Game Over";
			textWidth = electroliteFont->WidthText(text);
			textHeight = 120.f;
			electroliteFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + textHeight, text);
			text = "Your score was: " + std::to_string(score);
			textWidth = syneMonoFont->WidthText(text);
			textHeight = 40.f;
			syneMonoFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + textHeight, text);
			text = "Press ENTER to continue.";
			textWidth = syneMonoFont->WidthText(text);
			textHeight = -40.f;
			syneMonoFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + textHeight, text);
		}
		text = "Shields:";
		syneMonoFont->BlitText(hMArgin, blit3D->screenHeight - vMargin, text);
		textWidth = syneMonoFont->WidthText(text);
		for (int shieldCounter = 0; shieldCounter < ship->GetLives(); shieldCounter++)
		{
			shieldIconSprite->Blit(hMArgin + textWidth + 40.0f + (shieldCounter * 80.0f), 1030.f, 0.3f, 0.3f);
		}
		text = "Score: " + std::to_string(score);
		textWidth = syneMonoFont->WidthText(text);
		syneMonoFont->BlitText(blit3D->screenWidth - textWidth - hMArgin, blit3D->screenHeight - vMargin, text);
		if (levelTitleTimer < 2) {
			text = "Level " + std::to_string(level);
			textWidth = syneMonoFont->WidthText(text);
			syneMonoFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + 38.f, text);
		}
		text = "Guns:";
		textWidth = syneMonoFont->WidthText(text);
		syneMonoFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2 - (ship->GetPowerUp() * 25.0f) - 10.0f, blit3D->screenHeight - vMargin, text);
		for (int powerUpCounter = 0; powerUpCounter < ship->GetPowerUp(); powerUpCounter++)
		{
			shotInterfaceSprite->Blit(blit3D->screenWidth / 2 - textWidth / 2 - (ship->GetPowerUp() * 25.0f) + 10.0f + textWidth + (powerUpCounter * 50.0f), 1030.f, 0.3f, 0.3f);
		}
		break;
	default:
		break;
	}
	if (gameState == PAUSE)
	{
		// Draw the paused message
		text = "PAUSED";
		textWidth = electroliteFont->WidthText(text);
		textHeight = 76.f;
		electroliteFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + textHeight, text);

	}
}

/**
* This method handles the input.
*/
void DoInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence
	switch (gameState)
	{
	case TITLE_PAGE:
		// Start a new Game
		if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
		{
			if (ship != NULL) delete ship;
			for (auto asteroid : asteroids)
			{
				//asteroid->willDelete();
				if (asteroid != NULL) delete asteroid;
			}
			asteroids.clear();
			for (auto powerUp : powerUpList)
			{
				if (powerUp != NULL) delete powerUp;
			}
			powerUpList.clear();
			score = 0;
			level = 0;
			gameOver = false;
			powerUpList.clear();
			asteroids.clear();
			shotList.clear();
			levelTitleTimer = 0;
			level = 1;
			lastPowerUp = 0;
			notPlayedExplosion = true;
			//create a ship
			
			ship = new Spaceship(glm::vec2(backgroundWidth / 2, backgroundHeight / 2), blit3D->MakeSprite(0, 0, 100, 100, "Media\\shot.png"), 50.0f, 0.0f, audioE);
			//load a sprite off of a spritesheet
			ship->AddSprite(blit3D->MakeSprite(0, 0, 1452, 2180, "Media\\ship.png"));
			ship->AddSprite(blit3D->MakeSprite(1464, 0, 1452, 2180, "Media\\ship.png"));
			ship->AddSprite(blit3D->MakeSprite(2929, 0, 1452, 2180, "Media\\ship.png"));
			ship->AddSprite(blit3D->MakeSprite(4393, 0, 1452, 2180, "Media\\ship.png"));
			ship->SetShieldSprite(blit3D->MakeSprite(0, 0, 1781, 1473, "Media\\shield.png"));
			//load Asteroids
			for (int i = 0; i < level; i++)
			{
				Asteroid* asteroid = new Asteroid(BIG_ASTEROID, spriteLists, GetRandomPosition(backgroundWidth, backgroundHeight), random.RandomFloat(1, 10, 1), audioE);
				asteroid->SetVelocity({ random.RandomFloat(-50, 50, 1),random.RandomFloat(-50, 50, 1) });
				asteroids.push_back(asteroid);
			}
			gameState = GAME;
			audioE->StopEvent("TitleMusic", mainGameID, titleMusicId);
			gameMusicId = audioE->PlayEvent("GameMusic", mainGameID);
		}
		break;
	case GAME:
		if (!gameOver) 
		{
			AKRESULT panningX = AK::SoundEngine::SetRTPCValue(L"PanningX", (AkRtpcValue)(ship->GetPosition().x), mainGameID);
			// Movement controls
			if (key == GLFW_KEY_A && action == GLFW_PRESS)
				ship->SetTurnLeft(true);

			if (key == GLFW_KEY_A && action == GLFW_RELEASE)
				ship->SetTurnLeft(false);

			if (key == GLFW_KEY_D && action == GLFW_PRESS)
				ship->SetTurnRight(true);

			if (key == GLFW_KEY_D && action == GLFW_RELEASE)
				ship->SetTurnRight(false);

			if (key == GLFW_KEY_W && action == GLFW_PRESS && !ship->IsDestroyed()) {
				thrustSound = audioE->PlayEvent("Thrust", mainGameID);
				ship->SetThrusting(true);
			}
				

			if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
				audioE->StopEvent("Thrust", mainGameID, thrustSound);
				ship->SetThrusting(false);
			}
				
			// Shooting control
			if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
				shoot = true;
			}
				
			if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
				shoot = false;
			// Pause action
			if (key == GLFW_KEY_P && action == GLFW_RELEASE)
			{
				pauseSound = audioE->PlayEvent("Pause", mainGameID);
				gameState = PAUSE;
				audioE->PauseEvent("GameMusic", mainGameID, gameMusicId);
				audioE->PauseEvent("Thrust", mainGameID, thrustSound);
				ship->Pause();

			}
		}
		if (gameOver)
		{
			// Go to title page on gameover
			if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
			{
				score = 0;
				gameState = TITLE_PAGE;
				audioE->StopEvent("GameMusic", mainGameID, gameMusicId); 
				titleMusicId = audioE->PlayEvent("TitleMusic", mainGameID);
			}
		}
		break;
	case PAUSE:
		// Unpause action
		if (key == GLFW_KEY_P && action == GLFW_RELEASE)
		{
			pauseSound = audioE->PlayEvent("Pause", mainGameID);
			gameState = GAME;
			audioE->ResumeEvent("GameMusic", mainGameID, gameMusicId);
			audioE->ResumeEvent("Thrust", mainGameID, thrustSound);
			ship->Resume();
			if (thrustPressed) {
				thrustSound = audioE->PlayEvent("Thrust", mainGameID);
				ship->SetThrusting(true);
			}
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			thrustPressed = true;
		}

		if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
			audioE->StopEvent("Thrust", mainGameID, thrustSound);
			ship->SetThrusting(false);
			thrustPressed = false;
		}
		break;
	default:
		break;
	}
}

/**
* This method is called at the start.
*/
int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 77263;
	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 1920, 1080);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	
	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}