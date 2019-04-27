#ifndef _TEXT_GO_2D_H_
#define _TEXT_GO_2D_H_
#include "GameObject2D.h"
#include "GameData.h"

class TextGO2D :public GameObject2D
{
public:
	TextGO2D(string _text);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

	void SetText(string _Text) { m_text = _Text; }

	std::string TextSetup(GameData* _GD);
protected:
	string m_text;
};

#endif