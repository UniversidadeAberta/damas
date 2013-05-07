#pragma once
#include "casa.h"
#include "caminho.h"
#include "jogada.h"
#include <vector>

class Tabuleiro
{
	
	std::vector<std::vector<Caminho>> jogadasobrigatorias;
	std::vector<std::vector<Jogada>> jog;
public:
	enum erro{OK = 0, OFFBOARD, BLOCKED, DEFENDED, OTHER, ILLEGALMOVE, MANDATORYMOVE};
	enum diagonalDirection{NW=0, SW, NE, SE, ERRO};
	Casa *casas[8][8]; 
	std::vector<std::vector<Jogada>> JogadasObrigatorias();

	//construtor
	Tabuleiro(void);
	~Tabuleiro(void);
	

	//Validações
	bool ValidaMargens(int linha, int coluna, int dist);
	erro PodeComer(Casa from, Casa to, diagonalDirection dir );
	erro PodeComerDama(Casa from, Casa to, diagonalDirection dir);
	bool JogadaObrigatoria(Casa* from);
	void JogadaObrigatoriaPeao(Casa* from, diagonalDirection dir, Caminho* path);
	void JogadaObrigatoriaDama(Casa* from, diagonalDirection dir, Caminho* path, Dama* dama = NULL);
	void Tabuleiro::ComePeca(Casa* from, Casa* to);
	erro Move(Casa* from, Casa* to);
	void SelecionaCasa(int linha, int coluna);
	diagonalDirection Tabuleiro::DescobreDireccao(Casa c1, Casa c2);
	void AvancaDiagonal(int* linha, int*coluna, diagonalDirection direction);
	bool DamaPodeContinuar(Casa* casa, diagonalDirection dir, Dama* d);
};

