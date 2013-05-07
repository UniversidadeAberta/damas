#pragma once

class Cordenada
{
	int linha, coluna;
public:
	Cordenada(void);
	Cordenada(int linha, int coluna);
	~Cordenada(void);

	void Set(int x, int y);
	int GetX();
	int GetY();
};
