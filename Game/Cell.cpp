#include "Cell.h"

Cell::Cell(Vector3 _Pos, Vector3 _Scale, ID3D11Device* _pd3Device) : GameObject ()
{
	m_NPC = false;
	m_goal = false;
	m_object = false;
	m_taken = false;

	m_GridLocX = _Pos.x;
	m_GridLocY = _Pos.y;

	m_TotalMoveCost = 0;
	m_EstMoveCost = 0;
	m_MoveCost = 0;

	m_GridLoc = _Pos;

	//m_pos = Vector2((_Pos.x * 50) + 0.5f, (_Pos.y * 50) + 0.5f);
	//m_scale = _Scale;

	// set up locatin to display the values in each cell.
	//m_Text = new Text(std::to_string(0), Vector2((_Pos.x * 50) + 0.5f, (_Pos.y * 50) + 0.5f), _Scale / 2, Color(255, 69, 0));

	std::cout << ".";

	VBCube* m_Cube = new VBCube();
	m_Cube->init(20, _pd3dDevice);
	m_Cube->SetPos(Vector3(0.0f, 0.0f, 0.0f));
	m_Cube->SetScale(1.0f);
	m_GO = m_Cube;

}

Cell::~Cell()
{

}

void Cell::Tick(GameData * _GD)
{
	
}

void Cell::Draw(DrawData* _DD)
{

		m_GO->Draw(_DD);
}
