#pragma once
#include "cordenada.h"
class Jogada
{
	int x1, x2;
	int y1, y2;
public:
	Jogada(int origemX, int origemY, int destinoX, int destinoY);
	~Jogada(void);

	Cordenada GetOrigem();
	Cordenada GetDestino();
	bool Compara(Jogada j);
};

