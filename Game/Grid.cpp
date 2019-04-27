#include "Grid.h"
#include <iostream>


/*
Creates the grid and sets up the posistion, scale and Game Type.
Creates the NPC, Goal, Objects and Path classes, populate them with the images
*/
/*Grid::Grid(ID3D11Device* _pd3dDevice, GameData* _GD) : GameObject2D()
{
	int m_VecLoc = 0; //location within the vector
	for (int m_height = 0; m_height < _GD->m_scrHeight; m_height++)
	{
		for (int m_width = 0; m_width < _GD->m_scrWidth; m_width++)
		{ //Creates all cells, populates all cells with objects and path images.
			m_Cell.push_back(new Cell(Vector2(m_width, m_height), Vector2(0.5f, 0.5f), m_VecLoc, GT_FREE, "Empty", _pd3dDevice));
			m_Path.push_back(new Path("Path", _pd3dDevice, Vector2(m_width, m_height), Vector2(0.5f, 0.5f), m_VecLoc));
			m_Object.push_back(new Object("Object", _pd3dDevice, Vector2(m_width, m_height), Vector2(0.5f, 0.5f), m_VecLoc));
			m_VecLoc++;
		}
	}

	//m_ExcCell is the extra Cell, only used when calculating parents, 
	//when the parent adjacent is more than the width or height
	m_ExCell = new Cell(Vector2(0, 0), Vector2(1, 1), m_Cell.size() + 1, GT_NULL, "Empty", _pd3dDevice);

	FindParents(_GD); //fills the list of parents in the vector

	Vector2 m_tempLoc = Check(_GD); //checks for an open slot within the grid
	m_NPC = new NPC("NPC", _pd3dDevice, m_tempLoc, Vector2(0.5f, 0.5f)); // places NPC
	SetLocation(m_tempLoc, GT_NPC);

	m_tempLoc = Check(_GD);
	m_Goal = new Goal("Goal", _pd3dDevice, m_tempLoc, Vector2(0.5f, 0.5f)); // places goal
	SetLocation(m_tempLoc, GT_GOAL);

	SetUpObject(_GD);

	UpdatePos(); //Makes sure all objects are active to render

	m_DiagValue = 14;
	m_MoveValue = 10;
	m_EstMoveValue = 10;




}

Grid::~Grid()
{
	Cleancost();
	CleanVectors();
};

/*auto Grid::PollInput()
{
return std::async(std::launch::async, [] {
while (!m_exit)
{
input.store(_getch());
}
});
}*/

/*Sets the location of object*/
/*void Grid::SetLocation(Vector2 _Pos, GameType _GT)
{
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		if ((*it)->GetCellPos() == _Pos)
		{
			(*it)->setGameType(_GT);
		}
	}
}


void Grid::Tick(GameData * _GD)
{
	//if X amount of seonds have passed then enter tick
	m_TimeEnd = std::chrono::steady_clock::now();
	m_Totaltime = m_TimeEnd - _GD->m_TimeValue;


	if ((m_Totaltime.count() >= _GD->m_t) && !_GD->m_Pause)
	{
		switch (_GD->m_GS)
		{
		case GS_COSTSETUP: //calculate the cost from goal
			EstMoveCost(_GD, Find(GT_GOAL));
			setNPCLoc(Find(GT_NPC));
			break;
		case GS_SCAN: //Scan from NPC searching for the goal
			MoveCost(_GD, getNPCLoc());
			break;
		case GS_PLAY: //Move the player
			MovePlayer(_GD);
			break;
		case GS_RESCAN: // replace the goal and clean all of the costs
			Cleancost();
			CleanVectors(); //clears the values in the open list and closed list
			RescanCleanCost();
			ReplaceGoal(_GD);
			_GD->m_GS = GS_COSTSETUP;
			break;
		case GS_CLEAN: //clean out the cost values
			RescanCleanCost();
			_GD->m_GS = GS_DYNAMIC_RESCAN;
			break;
		case GS_DYNAMIC_RESCAN: //recalculate the movement cost
			MoveCost(_GD, getNPCLoc());
			break;
		case GS_RESET://reset the map
					  // clean the map of all GS and set taken to false
			CleanGrid();
			CleanUpdate();
			// respwan NPC
			Vector2 m_tempLoc = Check(_GD);
			SetLocation(m_tempLoc, GT_NPC);
			// respawn Goal
			m_tempLoc = Check(_GD);
			SetLocation(m_tempLoc, GT_GOAL);
			// respawn Object
			SetUpObject(_GD);
			// set GS to costsetup
			_GD->m_GS = GS_COSTSETUP;
			break;
		}

		UpdatePos();

		//update the objects
		m_Goal->Tick(_GD);
		m_NPC->Tick(_GD);

		for (auto it = m_Object.begin(); it != m_Object.end(); it++)
		{
			(*it)->Tick(_GD);
		}
		for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
		{
			(*it)->Tick(_GD);
		}


		_GD->m_TimeValue = std::chrono::steady_clock::now(); //resets the timer


															 //Help menu CMD print grid
		switch (_GD->m_DebugPrint)
		{
		case 1: PrintGrid(_GD);
			break;
		case 2:PrintEstCost(_GD);
			break;
		case 3:PrintMoveCost(_GD);
			break;
		case 4:PrintTotalCost(_GD);
			break;
		}
	}

}



void Grid::Draw(DrawData2D* _DD2D)
{//Visuals
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		(*it)->Draw(_DD2D);
	}

	for (auto it = m_Path.begin(); it != m_Path.end(); it++)
	{
		(*it)->Draw(_DD2D);
	}

	m_Goal->Draw(_DD2D);
	m_NPC->Draw(_DD2D);

	for (auto it = m_Object.begin(); it != m_Object.end(); it++)
	{
		(*it)->Draw(_DD2D);
	}


}

void Grid::UpdatePos()
{//Updates the images position
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		switch ((*it)->getGameType())
		{
		case GT_NPC:m_NPC->setNPCPos((*it)->GetCellPos());
			break;
		case GT_GOAL:m_Goal->setGoalPos((*it)->GetCellPos());
			break;
		case GT_PATH:m_Path.at((*it)->GetVecLoc())->setIsAlive(true);
			break;
		case GT_OBJECT:(*it)->setObjectlLoc();
			m_Object.at((*it)->GetVecLoc())->setIsAlive(true);
			break;
		default: m_Path.at((*it)->GetVecLoc())->setIsAlive(false);
			break;
		}
	}
}

void Grid::CleanUpdate()
{//For reset to clean up the grid **Does not work**
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		switch ((*it)->getGameType())
		{
		case GT_NPC:m_NPC->setNPCPos((*it)->GetCellPos());
			break;
		case GT_GOAL:m_Goal->setGoalPos((*it)->GetCellPos());
			break;
		case GT_PATH:m_Path.at((*it)->GetVecLoc())->setIsAlive(false);
			break;
		case GT_OBJECT:(*it)->setObjectlLoc();
			m_Object.at((*it)->GetVecLoc())->setIsAlive(false);
			break;
		default: m_Path.at((*it)->GetVecLoc())->setIsAlive(false);
			break;
		}
	}
}


//Find Functions- Finds the gametype in vector
int Grid::Find(GameType _GT)
{
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		switch (_GT)
		{
		case GT_NPC:
			if ((*it)->getGameType() == _GT)
			{
				return (*it)->GetVecLoc();
			}
			break;
		case GT_GOAL:
			if ((*it)->getGameType() == _GT)
			{
				return (*it)->GetVecLoc();
			}
			break;
		case GT_OBJECT:
			if ((*it)->getGameType() == _GT)
			{
				return (*it)->GetVecLoc();
			}
			break;
		}
	}
}

//Finds the Vector2 coordinates in vector
int Grid::FindGridPos(Vector2 _Pos)
{
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		if ((*it)->GetCellPos() == _Pos)
		{
			return (*it)->GetVecLoc();
		}
	}
}

//Find the parents to each grid cell
void Grid::FindParents(GameData * _GD)
{
	//loop from start to end of the vector
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{

		int tempY = (*it)->GetCellPos().y;
		int tempX = (*it)->GetCellPos().x;

		/**KEY**/
		/*
		tempY-1, tempX-1 = topLeft
		tempY-1, tempX = topMid
		tempY-1, tempX+1 = topRight
		tempY, tempX-1 = midLeft
		tempY, tempX+1 = midRight
		tempY+1, tempX-1 = bottomLeft
		tempY+1, tempX = bottomMid
		tempY+1, tempX+1 = bottomRight
		*/

		//the value 193 is outside the vector size so if 
		//a scan finds this number then it will be skip

		//Top left
/*		if (!((tempY - 1) <= -1))
		{
			if (!((tempX - 1) <= -1))
			{
				(*it)->setParent(FindGridPos(Vector2(tempX - 1, tempY - 1)));
				(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX - 1, tempY - 1))));

			}
			else
			{
				(*it)->setParent(GetCellSize() + 1);
				(*it)->setParents(m_ExCell);
			}
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);

		}

		//Top Mid
		if (!((tempY - 1) <= -1))
		{
			(*it)->setParent(FindGridPos(Vector2(tempX, tempY - 1)));
			(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX, tempY - 1))));
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);
		}

		//top right
		if (!((tempY - 1) <= -1))
		{
			if (!((tempX + 1) >= (_GD->m_scrWidth)))
			{
				(*it)->setParent(FindGridPos(Vector2(tempX + 1, tempY - 1)));
				(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX + 1, tempY - 1))));
			}
			else
			{
				(*it)->setParent(GetCellSize() + 1);
				(*it)->setParents(m_ExCell);
			}
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);

		}

		//mid left
		if (!((tempX - 1) <= -1))
		{
			(*it)->setParent(FindGridPos(Vector2(tempX - 1, tempY)));
			(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX - 1, tempY))));
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);
		}

		//mid right
		if (!((tempX + 1) >= (_GD->m_scrWidth)))
		{
			(*it)->setParent(FindGridPos(Vector2(tempX + 1, tempY)));
			(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX + 1, tempY))));
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);
		}

		//bottom left
		if (!((tempY + 1) >= _GD->m_scrHeight))
		{
			if (!((tempX - 1) <= -1))
			{
				(*it)->setParent(FindGridPos(Vector2(tempX - 1, tempY + 1)));
				(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX - 1, tempY + 1))));
			}
			else
			{
				(*it)->setParent(GetCellSize() + 1);
				(*it)->setParents(m_ExCell);
			}
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);

		}

		//bottom mid
		if (!((tempY + 1) >= _GD->m_scrHeight))
		{
			(*it)->setParent(FindGridPos(Vector2(tempX, tempY + 1)));
			(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX, tempY + 1))));
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);
		}


		//bottom right
		if (!((tempY + 1) >= _GD->m_scrHeight))
		{
			if (!((tempX + 1) >= (_GD->m_scrWidth)))
			{
				(*it)->setParent(FindGridPos(Vector2(tempX + 1, tempY + 1)));
				(*it)->setParents(m_Cell.at(FindGridPos(Vector2(tempX + 1, tempY + 1))));
			}
			else
			{
				(*it)->setParent(GetCellSize() + 1);
				(*it)->setParents(m_ExCell);
			}
		}
		else
		{
			(*it)->setParent(GetCellSize() + 1);
			(*it)->setParents(m_ExCell);

		}


	}
}


// checks the grid for an open slot **rebuild** can make more efficiant
Vector2 Grid::Check(GameData * _GD)
{
	int m_width, m_height;
	do
	{
		m_width = random->RandGen(0, _GD->m_scrWidth);
		m_height = random->RandGen(0, _GD->m_scrHeight);

		for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
		{
			if (((m_width == (*it)->GetCellX()) && (m_height == (*it)->GetCellY())) && (!(*it)->getTaken()))
			{
				(*it)->setTaken(true); //set the cell to taken
				return Vector2(m_width, m_height);
			}
		}
	} while (1);
}

//add to open list
void Grid::OpenList(Cell* _OpenList)
{
	m_OpenList.push_back(_OpenList);
}



//checks the surrounding cells for the minimum value
int Grid::CheckMinValue(vector<int> _list)
{
	int m_value = 1000;
	int m_cellValue = 9;
	int m_Debuglist;

	for (int m_list = 0; m_list < _list.size(); m_list++)
	{
		if (_list.at(m_list) != m_Cell.size() + 1)
		{
			if (m_Cell.at(_list.at(m_list))->GetTotalMoveCost() != 0)
			{
				if (m_Cell.at(_list.at(m_list))->getGameType() == GT_FREE)
				{
					//if movecost is less than previous then this cell is cheapest in parent list
					/*if ((m_Cell.at(_list.at(m_list))->GetTotalMoveCost() >= m_value) && (m_list< _list.size()))
					{
					m_list ++ ;
					}*/
/*					if (m_Cell.at(_list.at(m_list))->GetTotalMoveCost() <= m_value)
					{
						m_value = m_Cell.at(_list.at(m_list))->GetTotalMoveCost();
						m_cellValue = m_list;
					}
				}
			}
		}
	}
	return m_cellValue;
}



//Cost Calculations
void Grid::MoveCost(GameData * _GD, int _VectorLoc)
{
	/*
	loc 0 = top left
	loc 1 = top mid
	loc 2 = top right
	loc 3 = mid left
	loc 4 = mid right
	loc 5 = bot left
	loc 6 = bot mid
	loc 7 = bot right
	*/

/*	int m_tempPos = _VectorLoc;
	vector<int>m_templist; //list of current parents

	OpenList(m_Cell.at(m_tempPos)); //add the current vector pos to the open list

	for (int m_parentlist = 0; m_parentlist < m_Cell.at(m_tempPos)->getParentSize(); m_parentlist++)
	{
		m_templist.push_back(m_Cell.at(m_tempPos)->getParent(m_parentlist));
	}

	for (int m_list = 0; m_list < m_Cell.at(m_tempPos)->getParentSize(); m_list++)
	{
		if (m_templist.at(m_list) != m_Cell.size() + 1) //vec pos is outside of the gird
		{
			if (m_Cell.at(m_templist.at(m_list))->getGameType() == GT_GOAL)
			{ // if the path is next to the goal then move on to next state
				OpenList(m_Cell.at(m_templist.at(m_list)));
				m_ClosedList = m_OpenList;
				_GD->m_GS = GS_PLAY;
				return;
			}
		}
	}

	MoveCostCalculate(m_templist, m_Cell.at(_VectorLoc)->GetMoveCost()); // set the movement cost around the current cell

	int m_ListLoc = CheckMinValue(m_templist);
	if (m_ListLoc == 9) //Value 9 is outside the Vectorsize, meaning it could not find the goal
	{
		_GD->m_GS = GS_RESCAN;
		GetCell((Find(GT_GOAL)))->setGameType(GT_FREE);
		return;
	}
	m_tempPos = m_templist.at(m_ListLoc); //update the temppos with new vector location 

	m_Cell.at(m_tempPos)->setGameType(GT_PATH); // set the position as a path

	m_templist.clear(); // clear the list for next turn

	setNPCLoc(m_tempPos); // update the temp vector in the header.

}

void Grid::MoveCostCalculate(vector<int>  _list, int _CurrentCost)
{
	//caluculate the movecost around current cell
	if (_list.at(0) != m_Cell.size() + 1)
	{
		if (m_Cell.at(_list.at(0))->getGameType() == GT_FREE)
		{
			if (m_Cell.at(_list.at(0))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(0))->SetMoveCost(/*m_Cell.at(_list.at(0))->GetMoveCost() +*/ /*m_DiagValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(0)));
			}
		}
	}
	if (_list.at(1) != m_Cell.size() + 1)
	{
		if (m_Cell.at(_list.at(1))->getGameType() == GT_FREE)
		{
			if (m_Cell.at(_list.at(1))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(1))->SetMoveCost(/*m_Cell.at(_list.at(1))->GetMoveCost() +*/ /*m_MoveValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(1)));
			}
		}
	}
	if (_list.at(2) != m_Cell.size() + 1)
	{
		if (m_Cell.at(_list.at(2))->getGameType() == GT_FREE)
		{
			if (m_Cell.at(_list.at(2))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(2))->SetMoveCost(/*m_Cell.at(_list.at(2))->GetMoveCost() +*/ /*m_DiagValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(2)));
			}
		}
	}
	if (_list.at(3) != m_Cell.size() + 1)
	{
		if ((m_Cell.at(_list.at(3))->getGameType() == GT_FREE))
		{
			if (m_Cell.at(_list.at(3))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(3))->SetMoveCost(/*m_Cell.at(_list.at(3))->GetMoveCost() +*/ /*m_MoveValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(3)));
			}
		}
	}
	if (_list.at(4) != m_Cell.size() + 1)
	{
		if (m_Cell.at(_list.at(4))->getGameType() == GT_FREE)
		{
			if (m_Cell.at(_list.at(4))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(4))->SetMoveCost(/*m_Cell.at(_list.at(4))->GetMoveCost() +*/ /*m_MoveValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(4)));
			}
		}

	}
	if (_list.at(5) != m_Cell.size() + 1)
	{
		if (m_Cell.at(_list.at(5))->getGameType() == GT_FREE)
		{
			if (m_Cell.at(_list.at(5))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(5))->SetMoveCost(/*m_Cell.at(_list.at(5))->GetMoveCost() +*/ /*m_DiagValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(5)));
			}
		}

	}
	if (_list.at(6) != m_Cell.size() + 1)
	{
		if (m_Cell.at(_list.at(6))->getGameType() == GT_FREE)
		{
			if (m_Cell.at(_list.at(6))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(6))->SetMoveCost(/*m_Cell.at(_list.at(6))->GetMoveCost() +*/ /*m_MoveValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(6)));
			}
		}

	}
	if (_list.at(7) != m_Cell.size() + 1)
	{
		if (m_Cell.at(_list.at(7))->getGameType() != GT_OBJECT)
		{
			if (m_Cell.at(_list.at(7))->GetMoveCost() == 0)
			{
				m_Cell.at(_list.at(7))->SetMoveCost(/*m_Cell.at(_list.at(7))->GetMoveCost() +*/ /*m_DiagValue + _CurrentCost);
				TotalMoveCost(m_Cell.at(_list.at(7)));
			}
		}
	}
}

void Grid::EstMoveCost(GameData * _GD, int _VectorLoc)
{

	int m_spartingPos = _VectorLoc;
	int m_tempPosY = _VectorLoc;
	int m_tempPosXLeft;
	int m_tempPosXRight;

	int m_estMoveTemp = 0;

	do
	{//scan up
		m_tempPosXLeft = m_tempPosY;
		m_tempPosXRight = m_tempPosY;
		do
		{ //scan left
			if (m_Cell.at(m_tempPosXLeft)->GetCellX() == 0)
			{
				break;
			}
			m_tempPosXLeft -= 1;
			m_Cell.at(m_tempPosXLeft)->SetEstMoveCost(m_Cell.at(m_tempPosXLeft + 1)->GetEstMoveCost() + m_EstMoveValue);
		} while (1);

		do
		{//scan right
			if (m_Cell.at(m_tempPosXRight)->GetCellX() == _GD->m_scrWidth - 1)
			{
				break;
			}
			m_tempPosXRight += 1;
			m_Cell.at(m_tempPosXRight)->SetEstMoveCost(m_Cell.at(m_tempPosXRight - 1)->GetEstMoveCost() + m_EstMoveValue);
		} while (1);

		m_tempPosY -= _GD->m_scrWidth;

		if (m_tempPosY <= -1)
		{
			break;
		}

		m_Cell.at(m_tempPosY)->SetEstMoveCost(m_Cell.at(m_tempPosY + _GD->m_scrWidth)->GetEstMoveCost() + m_EstMoveValue);

	} while (1);

	m_tempPosY = m_spartingPos; // resets the Ypos to just under the goal

	do
	{//scan down
		m_tempPosXLeft = m_tempPosY;
		m_tempPosXRight = m_tempPosY;
		do
		{ //scan left
			if (m_Cell.at(m_tempPosXLeft)->GetCellX() == 0)
			{
				break;
			}
			m_tempPosXLeft -= 1;
			m_Cell.at(m_tempPosXLeft)->SetEstMoveCost(m_Cell.at(m_tempPosXLeft + 1)->GetEstMoveCost() + m_EstMoveValue);
		} while (1);

		do
		{//scan right
			if (m_Cell.at(m_tempPosXRight)->GetCellX() == _GD->m_scrWidth - 1)
			{
				break;
			}
			m_tempPosXRight += 1;
			m_Cell.at(m_tempPosXRight)->SetEstMoveCost(m_Cell.at(m_tempPosXRight - 1)->GetEstMoveCost() + m_EstMoveValue);
		} while (1);

		m_tempPosY += _GD->m_scrWidth;

		if (m_tempPosY >= m_Cell.size())
		{
			break;
		}
		m_Cell.at(m_tempPosY)->SetEstMoveCost(m_Cell.at(m_tempPosY - _GD->m_scrWidth)->GetEstMoveCost() + m_EstMoveValue);
	} while (1);

	_GD->m_GS = GS_SCAN; //Move on to the next game state
}

void Grid::TotalMoveCost(Cell* _Cell)
{//Adds movecost to est cost to make the total move cost
	_Cell->SetTotalMoveCost(_Cell->GetMoveCost() + _Cell->GetEstMoveCost());
}


//Moves the player
void Grid::MovePlayer(GameData* _GD)
{
	if (m_ClosedList.at(1)->getGameType() == GT_GOAL) // if player hits goal
	{
		_GD->m_GS = GS_RESCAN;
	}
	else if (m_ClosedList.at(1)->getGameType() == GT_OBJECT) // if player hits object
	{
		_GD->m_GS = GS_CLEAN;
		setNPCLoc(Find(GT_NPC));
		return;
	}
	m_Cell.at(m_ClosedList.at(0)->GetVecLoc())->setGameType(GT_FREE); //find current NPC location and change it to free
	m_Cell.at(m_ClosedList.at(0)->GetVecLoc())->setTaken(false);
	m_Cell.at(m_ClosedList.at(1)->GetVecLoc())->setGameType(GT_NPC); // set the NPC to next location in closed list
	m_ClosedList.erase(m_ClosedList.begin()); //erase the beginning item as is not needed.




}


// Cleans out all of the cost's from each cell.
void Grid::Cleancost()
{
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		(*it)->SetMoveCost(0);
		(*it)->SetEstMoveCost(0);
		(*it)->SetTotalMoveCost(0);
	}
}

void Grid::RescanCleanCost()
{//cleans everything off of the grid **Reset does not work**
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		(*it)->SetMoveCost(0);

		(*it)->SetTotalMoveCost(0);

		if ((*it)->getGameType() == GT_PATH)
		{
			(*it)->setGameType(GT_FREE);
		}
		CleanVectors();
	}
}


// Move the goal posistion
void Grid::ReplaceGoal(GameData* _GD)
{
	m_Cell.at(FindGridPos(Check(_GD)))->setGameType(GT_GOAL);
}



//cursor posistion to place objects
void Grid::Cursor(GameData * _GD)
{
	long m_mouseX, m_mouseY;

	m_mouseX = _GD->m_mousePosX / 50;
	m_mouseY = _GD->m_mousePosY / 50;

	if (m_Cell.at(FindGridPos(Vector2(m_mouseX, m_mouseY)))->getGameType() != GT_OBJECT)
	{
		if (m_Cell.at(FindGridPos(Vector2(m_mouseX, m_mouseY)))->getGameType() != GT_NPC)
		{
			if (m_Cell.at(FindGridPos(Vector2(m_mouseX, m_mouseY)))->getGameType() != GT_GOAL)
			{
				m_Cell.at(FindGridPos(Vector2(m_mouseX, m_mouseY)))->setGameType(GT_OBJECT);
				m_Cell.at(FindGridPos(Vector2(m_mouseX, m_mouseY)))->setTaken(true);
				SetLocation(Vector2(m_mouseX, m_mouseY), GT_OBJECT);
			}
		}


	}

	_GD->m_MouseLeftClick = false;

}


//Clean and reset
void Grid::CleanGrid()
{
	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		(*it)->setGameType(GT_FREE);
		(*it)->setTaken(false);
	}
}

void Grid::SetUpObject(GameData* _GD)
{//Sets up onjects onto the Map **was going to be used in reset**
	for (int m_objectAmount = 0; m_objectAmount < _GD->m_scrWidth; m_objectAmount++)
	{
		Vector2 m_tempLoc = Check(_GD);
		SetLocation(m_tempLoc, GT_OBJECT);

	}
}




//The print functions are for visual debugging on the command line
void Grid::PrintGrid(GameData* _GD)
{
	std::cout << "Grid:" << std::endl;
	int i = 0;

	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		switch ((*it)->getGameType())
		{
		case GT_NPC:std::cout << "X";
			break;
		case GT_GOAL:std::cout << "G";
			break;
		case GT_OBJECT:std::cout << "O";
			break;
		case GT_PATH:std::cout << "-";
			break;
		case GT_FREE: std::cout << ".";
			break;

		}
		i++;
		if (i == _GD->m_scrWidth)
		{
			i = 0;
			std::cout << endl;
		}
	}

	std::cout << "\n";
}

void Grid::PrintEstCost(GameData* _GD)
{
	std::cout << "EstCost:" << std::endl;
	int i = 0;

	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		printf("%3d ", (*it)->GetEstMoveCost());

		i++;
		if (i == _GD->m_scrWidth)
		{
			i = 0;
			std::cout << endl;
		}
	}
}

void Grid::PrintMoveCost(GameData* _GD)
{
	std::cout << "EstCost:" << std::endl;
	int i = 0;

	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{

		printf("%3d ", (*it)->GetMoveCost());


		i++;
		if (i == _GD->m_scrWidth)
		{
			i = 0;
			std::cout << endl;
		}
	}
}

void Grid::PrintTotalCost(GameData* _GD)
{
	std::cout << "Total Cost:" << std::endl;
	int i = 0;

	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		if ((*it)->getGameType() == GT_OBJECT)
		{
			printf(" X  ");
		}
		else if ((*it)->getGameType() == GT_GOAL)
		{
			printf(" G  ");
		}
		else if ((*it)->getGameType() == GT_PATH)
		{
			//printf(" -  ");
			printf("%3d ", (*it)->GetTotalMoveCost());
		}
		else
		{
			printf("%3d ", (*it)->GetTotalMoveCost());
		}

		i++;
		if (i == _GD->m_scrWidth)
		{
			i = 0;
			std::cout << endl;
		}
	}
}

void Grid::PrintVecLoc(GameData *_GD)
{
	std::cout << "Total Cost:" << std::endl;
	int i = 0;

	for (auto it = m_Cell.begin(); it != m_Cell.end(); it++)
	{
		switch ((*it)->getGameType())
		{
		case GT_NPC: printf("%3d ", (*it)->GetVecLoc());
			break;
		case GT_GOAL: printf("%3d ", (*it)->GetVecLoc());
			break;
		case GT_OBJECT: printf("%3d ", (*it)->GetVecLoc());
			break;
		default: printf(" .  ");
			break;
		}


		i++;
		if (i == _GD->m_scrWidth)
		{
			i = 0;
			std::cout << endl;
		}
	}
}*/