#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

//=================================================================
//Possible GameStates
//=================================================================
enum GameState {
	//Core GS
	GS_NULL = 0,
	GS_COSTSETUP,
	GS_SCAN,
	GS_RESCAN,
	GS_PLAY,
	GS_CLEAN,
	GS_DYNAMIC_RESCAN,
	GS_RESET,

	//Text GS
	GS_BASICTEXT = 10,
	GS_HELPTEXT
};

#endif