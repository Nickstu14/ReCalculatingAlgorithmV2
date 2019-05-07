#ifndef _GRID_H_
#define _GRID_H_

#include "GameData.h"
#include "Cell.h"
#include <list>
#include "CMOGO.h"
//#include "NPC.h"
//#include "Goal.h"
//#include "Object.h"
//#include "Path.h"
//#include "Random.h"
//#include "GameType.h"
//#include "Timer.h"
#include<atomic>
#include<thread>
#include<future>
//#include <conio.h>
#include <ctime>
#include <iostream>
#include "gameobject.h"


//class Cell;
//class NPC;
//class Goal;
//class Object;
//class Path;
//class Random;
//class Timer;
//class CTime;
class GameObject;


class Grid : GameObject
{
public:
	Grid(ID3D11Device* _pd3dDevice, GameData* _GD);
	~Grid();

	virtual void Tick(GameData * _GD);
	virtual void Draw(DrawData * _DD);
	void UpdatePos();

	Cell* GetCell(int _location) { return m_Cell.at(_location); }
	unsigned int GetCellSize() { return m_Cell.size(); }

	
	void CleanVectors() { m_OpenList.clear(); m_ClosedList.clear(); }




	void Cursor(GameData* _GD);

	void CleanGrid();
	void CleanUpdate();



	//auto PollInput();
	std::atomic<char> input;
	std::atomic<bool> m_exit;


	//char m_Input;

private:
	vector<Cell*> m_Cell;
	vector<Cell*> m_OpenList;
	vector<Cell*> m_ClosedList;
	vector<Cell*> m_UnusedPath;

	Cell* m_ExCell;

	int m_DiagValue;
	int m_MoveValue;
	int m_EstMoveValue;

	int m_NPCVecLoc; // used to calculate the movement cost for the path

	//Timer m_t;

	int m_runthough = 0;

	std::chrono::steady_clock::time_point m_TimeEnd;
	std::chrono::duration<float> m_Totaltime;

};
#endif