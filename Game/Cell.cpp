#include "Cell.h"

Cell::Cell(Vector2 _Pos, Vector2 _Scale, int _VecLoc, GameState  _GT, string _fileName, ID3D11Device* _GD) : ImageGO2D(_fileName, _GD)
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

	m_pos = Vector2((_Pos.x * 50) + 0.5f, (_Pos.y * 50) + 0.5f);
	m_scale = _Scale;

	m_GT = _GT;

	// set up locatin to display the values in each cell.
	m_Text = new Text(std::to_string(0), Vector2((_Pos.x * 50) + 0.5f, (_Pos.y * 50) + 0.5f), _Scale / 2, Color(255, 69, 0));


}

Cell::~Cell()
{

}

void Cell::Tick(GameData * _GD)
{
	if (_GD->m_TextToggle)
	{
		m_textToggle = true;
	}
	else
	{
		m_textToggle = false;
	}
	//updates the test to display the total movecost.
	m_Text->setText(std::to_string(m_TotalMoveCost));
}

void Cell::Draw(DrawData2D* _DD2D)
{

	ImageGO2D::Draw(_DD2D);

	if (m_textToggle)
		m_Text->Draw(_DD2D);
}
