#ifndef _CELL_H_
#define _CELL_H_

#include "GameData.h"

//#include "GameType.h"
//#include "Text.h"
#include <vector>
#include <iostream>
#include "VBCube.h"


class Text;
class Gameobject;

class Cell : public GameObject
{
public:
	Cell(Vector3 _Pos, Vector3 _Scale, ID3D11Device* _pd3Device);
	~Cell();

	int GetCellX() { return m_GridLocX; }
	int GetCellY() { return m_GridLocY; }
	Vector3 GetCellPos() { return m_GridLoc; }
	int GetVecLoc() { return m_VecLoc; }

	void setNPCLoc() { m_NPC = true; }
	void setGoalLoc() { m_goal = true; }
	void setObjectlLoc() { m_object = true; }
	void setTaken(bool _Taken) { m_taken = _Taken; }

	bool getNPC() { return m_NPC; }
	bool getGoal() { return m_goal; }
	bool getObject() { return m_object; }
	bool getTaken() { return m_taken; }

	int GetMoveCost() { return m_MoveCost; }
	int GetEstMoveCost() { return m_EstMoveCost; }
	int GetTotalMoveCost() { return m_TotalMoveCost; }

	void SetMoveCost(int _MoveCost) { m_MoveCost = _MoveCost; }
	void SetEstMoveCost(int _EstMoveCost) { m_EstMoveCost = _EstMoveCost; }
	void SetTotalMoveCost(int _TotalMoveCost) { m_TotalMoveCost = _TotalMoveCost; }


	void setGameType(GameState _GT) { m_GT = _GT; }
	GameState  getGameType() { return m_GT; }

	void setParent(int _parentInt) { m_Parent.push_back(_parentInt); }
	void setParents(Cell * _Cell) { m_parents.push_back(_Cell); }

	int getParent(int _parentInt) { return m_Parent.at(_parentInt); }
	int getParentSize() { return m_Parent.size(); }

	virtual void Tick(GameData * _GD);
	virtual void Draw(DrawData *_DD);



private:
	GameState  m_GT;
	bool m_taken; // boolean for checks if taken or not
	bool m_NPC; // these bool arent needed
	bool m_goal; //
	bool m_object;

	// A* star calculations
	int m_TotalMoveCost; //
	int m_EstMoveCost; //
	int m_MoveCost; //

					//Location within the grid
	int m_GridLocX;
	int m_GridLocY;
	Vector3 m_GridLoc;

	int m_VecLoc; //location within the vector

	vector <int> m_Parent; //dont know which one to use yet 
	vector <Cell*> m_parents;  //so both will be implimented
	Cell* m_Path; //this will store the next cell in the path

	GameObject* m_GO;

	//Text * m_Text;
	bool m_textToggle;


};



#endif
