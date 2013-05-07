#pragma once
#include "tabuleiro.h"
#include "dama.h"
#include "jogada.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "caminho.h"

class Jogo
{
	enum jogador{JOGADOR1 = 0, JOGADOR2};
	bool AvaliaJogadasObrigatorias(Dama::cor c);
	int Erro;
	void SetDamasBrancas();
	void SetDamasPretas();
	Dama damasbrancas[12];
	Dama damaspretas[12];
	int contador;
	std::string status;
	std::vector<Jogada> jobrigatorias;
	std::vector<std::vector<Jogada>> obrig;

public:
	enum vitoria{JOGBRANCA=1, JOGPRETA, EMPATE};
	Jogo(void);
	~Jogo(void);
	Tabuleiro *tab;
	void InitTabuleiro();
	void Turno();
	bool ObrigadoComer();
	void AvaliaPromocao();
	void FazJogada(Jogada j);
	void VerificaVitoria();
	static int numerojogada;
	static int movimento;
	static jogador jogadoractivo;
	static vitoria vit;
	
	std::string GetError();
	std::string GetStatus();

};
