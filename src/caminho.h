#pragma once
#include "casa.h"
#include <stdlib.h>
#include <vector>
class Caminho
{
	int valor;
		Casa* origem;
		Casa* destino;
		Caminho* sw;
		Caminho* se;
		Caminho* nw;
		Caminho* ne;
public:
	Caminho(int valor = 0, Casa* origem = NULL, Casa* destino = NULL, Caminho* sw = NULL, Caminho* se = NULL, Caminho* nw = NULL, Caminho* ne = NULL);
	~Caminho(void);

	void PathValue(Caminho* node, std::vector<Caminho> path, std::vector<std::vector<Caminho>>* jogada);

	int GetValor();
	void SetValor(int v);
	Casa* GetOrigem();
	void SetOrigem(Casa* origem);
	Casa* GetDestino();
	void SetDestino(Casa* destino);
	Caminho* GetSW();
	void SetSW(Caminho* sw);
	Caminho* GetSE();
	void SetSE(Caminho* se);
	Caminho* GetNW();
	void SetNW(Caminho* nw);
	Caminho* GetNE();
	void SetNE(Caminho* ne);

	
};

