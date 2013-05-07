#include "jogada.h"
#include "cordenada.h"
#include "stdafx.h"

Jogada::Jogada(int origemX, int origemY, int destinoX, int destinoY)
{
	this->x1= origemX;
	this->y1= origemY;
	this->x2 = destinoX;
	this->y2 = destinoY;
}

Jogada::~Jogada(void)
{
}

Cordenada Jogada::GetOrigem()
{
	return Cordenada(this->x1, this->y1);
}

Cordenada Jogada::GetDestino()
{
	return Cordenada(this->x2, this->y2);
}

bool Jogada::Compara(Jogada j)
{
	if ((this->x1 == j.x1) && (this->x2 == j.x2) && (this->y1 == j.y1) && (this->y2 == j.y2))
	{
		return true;
	}
	return false;
}