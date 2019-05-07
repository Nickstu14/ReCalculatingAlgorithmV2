#include "Grid.h"
#include <iostream>


/*
Creates the grid and sets up the posistion, scale and Game Type.
Creates the NPC, Goal, Objects and Path classes, populate them with the images
*/
Grid::Grid(ID3D11Device* _pd3dDevice, GameData* _GD) : GameObject()
{
	std::cout << _GD->m_scrHeight << " Screen height " << _GD->m_scrWidth << " Screen Width";
	int m_VecLoc = 0;
	for (int m_Height = 0; m_Height < _GD->m_scrHeight; m_Height++)
	{
		for (int m_Width = 0; m_Width < _GD->m_scrWidth; m_Width++)
		{
			m_Cell.push_back(new Cell(Vector3(m_Width, m_Height, 0), Vector3(0.5f, 0.5f,0.5f), _pd3dDevice));
			
		}
		std::cout << endl;
	}

}

Grid::~Grid()
{

}

void Grid::Tick(GameData * _GD)
{

}

void Grid::Draw(DrawData* _DD)
{

	for (auto it = m_Cell.begin; it != m_Cell.end; it++)
	{
		(*it)->Draw(_DD);
	}

}