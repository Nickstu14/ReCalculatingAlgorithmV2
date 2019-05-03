#include "TextGO2D.h"
#include "DrawData2D.h"
#include "helper.h"

TextGO2D::TextGO2D(string _text)
{
	SetText(_text);
}



void TextGO2D::Tick(GameData* _GD)
{
	//SetText(TextSetup(_GD));
	GameObject2D::Tick(_GD);
}


void TextGO2D::Draw(DrawData2D* _DD)
{
	_DD->m_Font->DrawString(_DD->m_Sprites.get(), Helper::charToWChar(m_text.c_str()), m_pos, m_colour,m_rotation,m_origin,m_scale);
}

/*std::string TextGO2D::TextSetup(GameData* _GD)
{
	switch (_GD->m_TGS)
	{
		case GS_BASICTEXT :	return "A-Star algorithm\nPress F1 for help";
			break;
		case GS_HELPTEXT: return "ESC - exit";
			break;
	}

}*/