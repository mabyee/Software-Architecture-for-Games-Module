#include "gamecode.h"
#include "myinputs.h"
#include <time.h>
#include "errorlogger.h"
#include <math.h>
#include "shapes.h"
#include "enemy.h"
#include "ObjectManager.h"
#include "brickwall.h"
#include "SoundFX.h"
#include "computer.h"
#include "ammoBox.h"
#include "outerwall.h"
#include "boss.h"
#include "turret.h"
#include "RoamingRobot.h"
#include "gate.h"
#include <thread>

Game::Game()
{
   // No-op
}

Game::~Game()
{
   // No-op
}

Game Game::instance;    // Singleton instance

// This is repeated, called every frame.
// It will run either Update( ), MainMenu() or PauseMenu() depending on the
// game state
ErrorType Game::Main()

{
	//Flip and clear the back buffer
	MyDrawEngine* pTheDrawEngine= MyDrawEngine::GetInstance();
	pTheDrawEngine->Flip();
	pTheDrawEngine->ClearBackBuffer();

	ErrorType err=SUCCESS;

	switch(m_currentState)
	{
	case MENU:

		err= MainMenu();     // Menu at start of game
		break;
	case PAUSED:
		err = PauseMenu();   // Player has paused the game
		break;
	case RUNNING:           // Playing the actual game
		err= Update();
		break;
   case GAMEOVER:
		err = FAILURE;       // Error return causes the window loop to exit
	default:
		// Not a valid state
		err = FAILURE;       // Error return causes the window loop to exit
	}

	return err;
}

// Use to change the state of the game. Private function
void Game::ChangeState(GameState newState)
{
	// Very crude state system
	// Close old state
	switch(m_currentState)
	{
	case MENU:
      // Not needed
		break;
	case PAUSED:
      // Not needed
		break;
	case RUNNING:
      // Not needed
		break;
	}

	// Change the state
	m_currentState = newState;
	m_menuOption = 0;

	// Transition to new state
	switch(m_currentState)
	{
	case MENU:
      // Not needed
		break;
	case PAUSED:
      // Not needed
		break;
	case RUNNING:
      // Not needed
		break;
	}
}

// Starts the game engines - Draw Engine, Sound Engine, Input Engine - singletons
// This is called soon after the program runs
ErrorType Game::Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance)
{
	// Create the engines - this should be done before creating other DDraw objects
	if(FAILED(MyDrawEngine::Start(hwnd, bFullScreen)))
	{
		ErrorLogger::Writeln(L"Failed to start MyDrawEngine");
		return FAILURE;
	}
	if(FAILED(MySoundEngine::Start(hwnd)))
	{
		ErrorLogger::Writeln(L"Failed to start MySoundEngine");
		return FAILURE;
	}
	if(FAILED(MyInputs::Start(hinstance, hwnd)))
	{
		ErrorLogger::Writeln(L"Failed to start MyInputs");
		return FAILURE;
	}
	return (SUCCESS);
}



// Terminates the game engines - Draw Engine, Sound Engine, Input Engine
// This is called just before the program exits
void Game::Shutdown()

{
   // Any clean up code here 


	// (engines must be terminated last)
	MyDrawEngine::Terminate();
	MySoundEngine::Terminate();
	MyInputs::Terminate();
}

// **********************************************************************************************
// Placeholder menus  ***************************************************************************
// **********************************************************************************************

// Called each frame when in the pause state. Manages the pause menu
// which is currently a basic placeholder
ErrorType Game::PauseMenu()
{
	// Code for a basic pause menu

	MyDrawEngine::GetInstance()->WriteText(450,220, L"Paused", MyDrawEngine::WHITE);

	const int NUMOPTIONS = 2;
	wchar_t options[NUMOPTIONS][11] = {L"Resume", L"Main menu"};

   // Display menu options
	for(int i=0;i<NUMOPTIONS;i++)
	{
		int colour = MyDrawEngine::GREY;       // If not selected, should be grey
		if(i == m_menuOption)
		{
			colour = MyDrawEngine::WHITE;       // Current selection is white
		}
		MyDrawEngine::GetInstance()->WriteText(450,300+50*i, options[i], colour);
	}

	MyInputs* pInputs = MyInputs::GetInstance();

   // Get user input
	pInputs->SampleKeyboard();

   // Move choice up and down
	if(pInputs->NewKeyPressed(DIK_UP))
	{
		m_menuOption--;
	}
	if(pInputs->NewKeyPressed(DIK_DOWN))
	{
		m_menuOption++;
	}
	if(m_menuOption<0)
	{
		m_menuOption=0;
	}
	else if(m_menuOption>=NUMOPTIONS)
	{
		m_menuOption=NUMOPTIONS-1;
	}

   // If player chooses an option ....
	if(pInputs->NewKeyPressed(DIK_RETURN))
	{
		if(m_menuOption ==0)      // Resume
		{
			ChangeState(RUNNING);  // Go back to running the game
		}
		if(m_menuOption ==1)      // Quit
		{
			EndOfGame();           // Clear up the game
			ChangeState(MENU);     // Go back to the menu
		}

	}

	return SUCCESS;
}

// Called each frame when in the menu state. Manages the menu
// which is currently a basic placeholder
ErrorType Game::MainMenu()
{
	MyDrawEngine::GetInstance()->WriteText(450,220, L"Main menu", MyDrawEngine::WHITE);

	const int NUMOPTIONS = 2;
	const int NUMDESC = 7;
	const int NUMKEYS = 6;
	wchar_t options[NUMOPTIONS][15] = { L"Start game", L"Exit" };
	wchar_t description[NUMDESC][60] =
	{
		L"The objective of the game is shown",
		L"at the top of the screen below the score.",
		L"",
		L"Some game information:",
		L"- White blocks are destructible",
		L"- The cloud does tick-damage and should be avoided",
		L"- Enemies drop keycards"
	};
	wchar_t keys[NUMKEYS][50] =
	{
		L"Key Layout:",
		L"W, S - Movement",
		L"A, D - Rotation",
		L"SPACE - Shoot",
		L"F - Interact",
		L"E - Place Stinger"
	};
	// Display the options
	for (int i = 0; i < NUMOPTIONS; i++)
	{
		int colour = MyDrawEngine::GREY;
		if (i == m_menuOption)
		{
			colour = MyDrawEngine::WHITE;
		}
		MyDrawEngine::GetInstance()->WriteText(450, 300 + 50 * i, options[i], colour);
	}
	for (int i = 0; i < NUMDESC; i++)
	{
		int colour = MyDrawEngine::WHITE;
		MyDrawEngine::GetInstance()->WriteText(450, 500 + 25 * i, description[i], colour);
	}
	for (int i = 0; i < NUMKEYS; i++)
	{
		int colour = MyDrawEngine::WHITE;
		MyDrawEngine::GetInstance()->WriteText(450, 700 + 25 * i, keys[i], colour);
	}
   // Get keyboard input
	MyInputs* pInputs = MyInputs::GetInstance();

	pInputs->SampleKeyboard();
	if(pInputs->NewKeyPressed(DIK_UP))
	{
		m_menuOption--;
	}
	if(pInputs->NewKeyPressed(DIK_DOWN))
	{
		m_menuOption++;
	}
	if(m_menuOption<0)
	{
		m_menuOption=0;
	}
	else if(m_menuOption>=NUMOPTIONS)
	{
		m_menuOption=NUMOPTIONS-1;
	}

   // User selects an option
	if(pInputs->NewKeyPressed(DIK_RETURN))
	{
		if(m_menuOption ==0)          // Play
		{  
			StartOfGame();             // Initialise the game
			ChangeState(RUNNING);      // Run it
		}

		if(m_menuOption ==1)          //Quit
		{
			ChangeState(GAMEOVER);
		}
	}

	return SUCCESS;
}


// **********************************************************************************************
// The game !!! *********************************************************************************
// **********************************************************************************************


// Called at the start of the game - when changing state from MENU to RUNNING
// Use this to initialise the core game
ErrorType Game::StartOfGame()
{
   // Code to set up your game *********************************************
   // **********************************************************************

	// Loading background
	backgroundImage = MyDrawEngine::GetInstance()->LoadPicture(L"Images/Background/backgroundFloor.bmp");
// Procedurally generating the level ------------------------------------------------------------------------
	int numberRows = 23;
	int numberCol = 37;
	const int TILES = 851;
	float ang = 0.0f;
	int j = 0;
	int spawnCounter = 0;
	Vector2D pos;
	Vector2D initialArr[TILES]; 
	Vector2D endArr[TILES];
	Vector2D spawnArr[TILES];
	
	// Filling initialArr with every possible tile on the map (making a tile grid)
	for (int i = -1600; i <= 1600; i = i + BLOCK_SIZE) //width
	{
		for (int k = -1000; k <= 1000; k = k + BLOCK_SIZE) //height
		{
			if ((i <= -200 || i >= 200) || (k <= -200 || k >= 200)) // leaving middle of map blank
			{
				initialArr[j] = Vector2D(i, k);
			}
			j++;
		}
	}
	// --------Placing initial inner walls--------------
	
	for (int i = 0; i < TILES; i++)//loop through entire initialArr
	{
		// inner walls
		int n = rand() % 3 + 1; // 33% chance of creating a wall
		if (n == 1)
		{
			endArr[i] = initialArr[i];
		}
		else
		{
			if (initialArr[i] != Vector2D(0, 0) && !(initialArr[i].XValue == FIRST_X || initialArr[i].XValue == LAST_X || initialArr[i].YValue == FIRST_Y || initialArr[i].YValue == LAST_Y))
			{
				spawnArr[spawnCounter] = initialArr[i];
				spawnCounter++;
			}
		}
	}

	for (int i = 0; i < TILES; i++)
	{
		//if the wall is not at 0,0 and does not have a wall next to it, delete it
		if (endArr[i] != Vector2D(0, 0)  && endArr[i].XValue != endArr[i + 1].XValue && endArr[i].YValue != endArr[i + numberRows].YValue && endArr[i].YValue != endArr[i - numberRows].YValue && endArr[i].XValue != endArr[i - 1].XValue)
		{
			endArr[i] = Vector2D(0, 0);
		}
		//deleting any overlapping walls on outside
		if (endArr[i] != Vector2D(0, 0) && endArr[i].XValue == FIRST_X || endArr[i].XValue == LAST_X || endArr[i].YValue == FIRST_Y || endArr[i].YValue == LAST_Y)
		{
			endArr[i] = Vector2D(0, 0);
		}
	}

	// Placing walls where vector != (0,0)
	for (int i = 0; i < TILES; i++)
	{
		if (endArr[i] != Vector2D(0, 0))
		{
			BrickWall* pBrickWall = new BrickWall();
			pos = endArr[i];
			ang = 0.0f;
			pBrickWall->Initialise(pos, ang);
			ObjectManager.AddObject(pBrickWall);
		}
	}
	//outer walls
	outerwall* pOuterWallBot = new outerwall();
	pOuterWallBot->Initialise(Vector2D(FIRST_X, FIRST_Y), 0.0f, 37, 1);
	ObjectManager.AddObject(pOuterWallBot);

	outerwall* pOuterWallTop1 = new outerwall();
	pOuterWallTop1->Initialise(Vector2D(FIRST_X, LAST_Y), 0.0f, 18, 1);
	ObjectManager.AddObject(pOuterWallTop1);

	outerwall* pOuterWallTop2 = new outerwall();
	pOuterWallTop2->Initialise(Vector2D(-16 + 88, GATE_Y), 0.0f, 18, 1);
	ObjectManager.AddObject(pOuterWallTop2);

	outerwall* pOuterWallLeft = new outerwall();
	pOuterWallLeft->Initialise(Vector2D(FIRST_X, FIRST_Y), 0.0f, 1, 23);
	ObjectManager.AddObject(pOuterWallLeft);

	outerwall* pOuterWallRight = new outerwall();
	pOuterWallRight->Initialise(Vector2D(LAST_X, FIRST_Y), 0.0f, 1, 23);
	ObjectManager.AddObject(pOuterWallRight);

	// finished creating walls ------------------------------------------------------------------------------
	// Loading soundFX
	pTheSoundFX = new SoundFX();
	pTheSoundFX->LoadSounds();
	
	// Spawning turrets
	for (int i = 0; i < 2; i++)
	{
		Turret* pTurret = new Turret();
		Vector2D pos;
		pos = spawnArr[rand() % 450]; // random available spawn point
		pTurret->Initialise(pos, &ObjectManager, pTheSoundFX, &Score);
		ObjectManager.AddObject(pTurret);
	}

	// Spawning enemies
	for (int i = 0; i < 5; i++)
	{
		Enemy* pRobot = new Enemy();
		Vector2D pos;
		Vector2D vel;
		pos = spawnArr[rand() % 450]; // random available spawn point
		vel.set(rand() % 628 / 100.0f, rand() % 100 + 100);
		pRobot->Initialise(pos, vel, &ObjectManager, pTheSoundFX, &Score);
		ObjectManager.AddObject(pRobot);
	}
	
	// Spawning soldier
	pSoldier = new Soldier();
	Vector2D startPos(0, 0);
	pSoldier->Initialise(startPos, &ObjectManager, pTheSoundFX, &Score);

	// Spawning boss cloud
	Boss* pBoss = new Boss();
	pBoss->Initialise(spawnArr[rand() % 450], &ObjectManager, pTheSoundFX, pSoldier);
	
	// Spawning Roaming Robot
	RoamingRobot* pRoamingRobot = new RoamingRobot();
	pRoamingRobot->Initialise(spawnArr[rand() % 450], &ObjectManager, pTheSoundFX, pSoldier, &Score);
	
	// AmmoBoxes
	for (int i = 0; i < 4; i++)
	{
		ammoBox* pAmmoBox = new ammoBox();		
		Vector2D pos;
		pos = spawnArr[rand() % 450]; // random available spawn point
		pAmmoBox->Initialise(pos, &ObjectManager);
		ObjectManager.AddObject(pAmmoBox);
	}

	// Placing Gate
	gate* pGate = new gate();
	pos = Vector2D(-16, LAST_Y); //fixed position
	pGate->Initialise(pos, pSoldier);
	ObjectManager.AddObject(pGate);

	// HUD
	HUD.Initialise(pSoldier);

	// Computer
	for (int i = 0; i < 4; i++)
	{
		computer* pComputer = new computer();
		Vector2D pos;
		pos = spawnArr[rand() % 450]; // random available spawn point
		pComputer->Initialise(pos, 0.4f);
		ObjectManager.AddObject(pComputer);
	}

	ObjectManager.AddObject(pRoamingRobot);
	ObjectManager.AddObject(pSoldier);
	ObjectManager.AddObject(pBoss); //loading cloud boss last to render after player
	

	gt.mark();
	gt.mark();

	return SUCCESS;
}

// Called each frame when in the RUNNING state.
// Checks for user pressing escape (which puts the game in the PAUSED state)
// Flips and clears the back buffer
// Gameplay programmer will develop this to create an actual game
ErrorType Game::Update()
{
	// Check for entry to pause menu
	static bool escapepressed = true;
	if(KEYPRESSED(VK_ESCAPE))
	{
		if(!escapepressed)
			ChangeState(PAUSED);
		escapepressed=true;
		MySoundEngine* pSE = MySoundEngine::GetInstance();
		pSE->StopAllSounds(); //stop all sounds when entering menu
	}
	else
		escapepressed=false;

	
   // Your code goes here *************************************************
   // *********************************************************************

	gt.mark();

	MyDrawEngine::GetInstance()->DrawAt(Vector2D(0,0), backgroundImage, 0.95f, 0.0f, 0.0f); // Drawing background
	ObjectManager.RenderAll();
	ObjectManager.UpdateAll(gt.mdFrameTime);
	ObjectManager.DeleteInactiveObjects();
	ObjectManager.CheckAllCollisions();
	
	HUD.Update(Score.GetScore()); // Drawing HUD

	// *********************************************************************
   // *********************************************************************
	if (m_currentState == GameState::MENU)
	{
		EndOfGame();
	}

	return SUCCESS;
}

// Called when the player ends the game
// Currently this is done from the PAUSED state, when returning to the main menu
// but could be done by the gameplay programmer in other situations
// This will be used by the gameplay programmer to clean up
ErrorType Game::EndOfGame()
// called when the game ends by returning to main menu
{
   // Add code here to tidy up ********************************************
   // *********************************************************************
	Score.ResetScore();
	delete pTheSoundFX;
	ObjectManager.DeleteAll();
	return SUCCESS;
}

