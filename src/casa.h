#pragma once
#include "dama.h"

class Casa
{
	Dama* dama;
	int linha;
	int coluna;
	bool selected;
public:
	Casa(void);
	Casa(int linha, int coluna);
	Casa(int linha, int coluna, Dama dama);
	~Casa(void);

	int GetLinha();
	int GetColuna();
	void SetLinha(int linha);
	void SetColuna(int coluna);
	void SetPosition(int linha, int coluna);
	bool isSelected();
	void Select();
	void Unselect();
	Dama* GetContent();
	void SetContent(Dama* p);
	bool hasContent();
	bool isDiagonal(Casa* to);
	int DistanceTo(Casa* to);
};

