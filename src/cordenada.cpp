#include "StdAfx.h"
#include "cordenada.h"

Cordenada::Cordenada(void)
{
}

Cordenada::Cordenada(int linha, int coluna)
{
	this->linha = linha;
	this->coluna = coluna;
}

Cordenada::~Cordenada(void)
{
}

void Cordenada::Set(int x, int y)
{
	this->linha = x;
	this->coluna = y;
}

int Cordenada::GetX()
{
	return this->linha;
}

int Cordenada::GetY()
{
	return this->coluna;
}
