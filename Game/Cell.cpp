#include "Cell.h"

Cell::Cell(Vector2 _Pos, Vector2 _Scale, int _VecLoc, GameState  _GT, ID3D11Device* _pd3dDevice, string _fileName, IEffectFactory* _EF) : CMOGO(_fileName,_pd3dDevice,_EF)
{
	m_NPC = false;
	m_goal = false;
	m_object = false;
	m_taken = false;

	m_GridLocX = _Pos.x;
	m_GridLocY = _Pos.y;
	m_VecLoc = _VecLoc;

	m_TotalMoveCost = 0;
	m_EstMoveCost = 0;
	m_MoveCost = 0;

	m_GridLoc = _Pos;

	//m_pos = Vector2((_Pos.x * 50) + 0.5f, (_Pos.y * 50) + 0.5f);
	//m_scale = _Scale;

	m_GT = _GT;

	// set up locatin to display the values in each cell.
	//m_Text = new Text(std::to_string(0), Vector2((_Pos.x * 50) + 0.5f, (_Pos.y * 50) + 0.5f), _Scale / 2, Color(255, 69, 0));

	std::cout << ".";

	m_Cube = new VBCube();
	m_Cube->init(20, _GD);
	m_Cube->SetPos(Vector3(0.0f, 0.0f, 0.0f));
	m_Cube->SetScale(1.0f);
	

}

Cell::~Cell()
{

}

void Cell::Tick(GameData * _GD)
{
	
}

void Cell::Draw(DrawData* _DD)
{

		m_Cube->Draw(_DD);
}
