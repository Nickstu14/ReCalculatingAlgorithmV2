#ifndef _GRID_H_
#define _GRID_H_

/*#include "GameData.h"
//#include "Cell.h"
#include <list>
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


class Cell;
class NPC;
class Goal;
class Object;
class Path;
class Random;
class Timer;
class CTime;

class Grid : public GameObject2D
{
public:
	Grid(ID3D11Device* _pd3dDevice, GameData* _GD);
	~Grid();

	virtual void Tick(GameData * _GD);
	virtual void Draw(DrawData2D * _DD2D);
	void UpdatePos();

	Vector2 Check(GameData* _GD);

	//void SetUpNPC();
	//void SetUpGoal();
	//void SetUpObject(GameData* _GD);
	//void SetLocation(Vector2 _Pos, GameType _GT);

	Cell* GetCell(int _location) { return m_Cell.at(_location); }
	unsigned int GetCellSize() { return m_Cell.size(); }

	void PrintGrid(GameData *_GD);
	void PrintEstCost(GameData* _GD);
	void PrintMoveCost(GameData* _GD);
	void PrintTotalCost(GameData* _GD);
	void PrintVecLoc(GameData *_GD);


	void MoveCost(GameData * _GD, int _VectorLoc);
	void EstMoveCost(GameData * _GD, int _VectorLoc);
	void TotalMoveCost(Cell* _Cell);
	void MoveCostCalculate(vector<int>  _List, int _CurrentCost);
	int CheckMinValue(vector<int> _list);

	void OpenList(Cell* _OpenList);

	int Find(GameType _GT);
	int FindGridPos(Vector2 _Pos);
	void FindParents(GameData* _GD);

	void MovePlayer(GameData* _GD);
	void setNPCLoc(int _tempLoc) { m_NPCVecLoc = _tempLoc; }
	int getNPCLoc() { return m_NPCVecLoc; }

	void Cleancost();
	void RescanCleanCost();
	void CleanVectors() { m_OpenList.clear(); m_ClosedList.clear(); }

	void ReplaceGoal(GameData* _GD);


	void Cursor(GameData* _GD);

	void CleanGrid();
	void CleanUpdate();



	//auto PollInput();
	std::atomic<char> input;
	std::atomic<bool>m_exit;


	//char m_Input;

private:
	vector<Cell*> m_Cell;
	vector<Cell*> m_OpenList;
	vector<Cell*> m_ClosedList;
	vector<Cell*> m_UnusedPath;

	Cell* m_ExCell;

	NPC * m_NPC;
	Goal * m_Goal;
	vector<Object *> m_Object;
	vector<Path*> m_Path;



	Random * random;

	int m_DiagValue;
	int m_MoveValue;
	int m_EstMoveValue;

	int m_NPCVecLoc; // used to calculate the movement cost for the path

	Timer m_t;

	int m_runthough = 0;

	std::chrono::steady_clock::time_point m_TimeEnd;
	std::chrono::duration<float> m_Totaltime;

};*/
#endif