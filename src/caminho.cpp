#include "stdafx.h"
#include "caminho.h"

Caminho::Caminho(int valor, Casa* origem, Casa* destino, Caminho* sw , Caminho* se, Caminho* nw, Caminho* ne)
{
	this->valor = valor;
	this->destino = destino;
	this->origem = origem;
	this->sw = sw;
	this->se = se;
	this->nw = nw;
	this->ne = ne;
}


Caminho::~Caminho(void)
{
}

void Caminho::PathValue(Caminho* node, std::vector<Caminho> path, std::vector<std::vector<Caminho>>* jogada) 
{
	if (node==NULL) return;

	if( node->valor != 0)
	{
		 path.push_back(*node);
	}

	// it's a leaf, so print the path that led to here
	if ((node->sw==NULL && node->se==NULL) && (node->nw==NULL && node->ne==NULL)) 
	{
		jogada->push_back(path);
	}
	else 
	{
		
		PathValue(node->sw, path, jogada);
		PathValue(node->se, path, jogada);
		PathValue(node->nw, path, jogada);
		PathValue(node->ne, path, jogada);
	}
} 

int Caminho::GetValor()
{
	return this->valor;
}

void Caminho::SetValor(int v)
{
	this->valor = v;
}

Casa* Caminho::GetOrigem()
{
	return this->origem;
}

void Caminho::SetOrigem(Casa* origem)
{
	this->origem = origem;
}

Casa* Caminho::GetDestino()
{
	return this->destino;
}

void Caminho::SetDestino(Casa* destino)
{
	this->destino = destino;
}

Caminho* Caminho::GetSW()
{
	return this->sw;
}

void Caminho::SetSW(Caminho* sw)
{
	this->sw = sw;
}

Caminho* Caminho::GetSE()
{
	return this->se;
}

void Caminho::SetSE(Caminho* se)
{
	this->se = se;
}

Caminho* Caminho::GetNW()
{
	return this->nw;
}

void Caminho::SetNW(Caminho* nw)
{
	this->nw = nw;
}

Caminho* Caminho::GetNE()
{
	return this->ne;
}

void Caminho::SetNE(Caminho* ne)
{
	this->ne = ne;
}