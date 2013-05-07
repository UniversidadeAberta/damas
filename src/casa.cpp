#include "casa.h"
#include "dama.h"
#include <stdlib.h>
#include "stdafx.h"

Casa::Casa(void)
{
}

Casa::Casa(int linha, int coluna)
{
	this->coluna = coluna;
	this->linha = linha;
}

Casa::Casa(int linha, int coluna, Dama dama)
{
	this->coluna = coluna;
	this->linha = linha;
	this->dama = &dama;
}


Casa::~Casa(void)
{
}

int Casa::GetLinha()
{
	return this->linha;
}

int Casa::GetColuna()
{
	return this->coluna;
}

void Casa::SetLinha(int lina)
{
	this->linha = linha;
}

void Casa::SetColuna(int coluna)
{
	this->coluna = coluna;
}

void Casa::SetPosition(int linha, int coluna)
{
	this->coluna = coluna;
	this->linha = linha;
}

Dama* Casa::GetContent()
{
	if(this->hasContent())
	{
		return this->dama;
	}
	else
	{
		return NULL;
	}
}

void Casa::SetContent(Dama *p)
{
	this->dama = p;
}

bool Casa::hasContent()
{
	if(this->dama != NULL)
	{
		return true;
	}
	return false;
}

bool Casa::isSelected()
{
	return this->selected = true;
}

void Casa::Select()
{
	this->selected = true;
}

void Casa::Unselect()
{
	this->selected = false;
}

bool Casa::isDiagonal(Casa* to)
{
	Casa* check = to;
	if(abs(this->linha - check->linha) == abs(check->coluna- this->coluna))
	{
		return true;
	}
	return false;
}

int Casa::DistanceTo(Casa* to)
{
	Casa check = *to;
	return check.linha - this->linha;
}

