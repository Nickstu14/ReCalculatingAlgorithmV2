#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

//=================================================================
//Data to be passed by game to all Game Objects via Tick
//=================================================================
#include <dinput.h>
#include "Keyboard.h"
#include "Mouse.h"
#include "GameState.h"
#include "SimpleMath.h"
#include <chrono>

using namespace DirectX;

struct GameData
{
	float m_dt;  //time step since last frame
	GameState m_GS; //global GameState
	GameState m_TGS; //Text Gamestate

	float m_t; //for the Timer class max amount of seconds for delay
	bool m_Pause; //pauses game loop
	std::chrono::steady_clock::time_point m_TimeValue;

	//player input
	unsigned char* m_keyboardState; //current state of the Keyboard
	unsigned char* m_prevKeyboardState; //previous frame's state of the keyboard
	DIMOUSESTATE* m_mouseState; //current state of the mouse

	int m_scrWidth;
	int m_scrHeight;

	int m_mousePosX; // mouse's posistion
	int m_mousePosY;
	bool m_MouseLeftClick; // mouses left click

	bool m_TextToggle;	//toggles the display of the text
	bool m_HelpText;	//Toggles the help menu
	bool m_F1;			//Toggles off when help menu is shown
	bool m_Reset;		//Resets the map
	int m_DebugPrint;
};
#endif
