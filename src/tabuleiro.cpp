#include "stdafx.h"
#include "tabuleiro.h"
#include <stdlib.h>
#include <Vector>
#include "jogada.h"


Tabuleiro::Tabuleiro(void)
{
	//Cosntruir tabuleiro iniciando as casas
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			this->casas[i][j] = new Casa();
			this->casas[i][j]->SetPosition(i,j);
			this->casas[i][j]->SetContent(NULL);
		}
	}
}

Tabuleiro::~Tabuleiro(void)
{
}

// Simular pontos cardeais para navegação da diagonal
void Tabuleiro::AvancaDiagonal(int* linha, int*coluna, diagonalDirection direction)
{
	int lineSum = 0;
	int colSum = 0;
	switch(direction)
	{
		case NW:
			lineSum = 1;
			colSum = -1;
			break;
		case NE:
			lineSum = 1;
			colSum = 1;
			break;
		case SE:
			lineSum = -1;
			colSum = 1;
			break;
		case SW:
			lineSum= -1;
			colSum = -1;
			break;
	}
	*linha = *linha+ lineSum;
	*coluna = *coluna + colSum;
}

// Metodo para descubrir qual a direcção da jogada
Tabuleiro::diagonalDirection Tabuleiro::DescobreDireccao(Casa c1, Casa c2)
{
	int linha1 = c1.GetLinha();
	int coluna1 = c1.GetColuna();
	int linha2 = c2.GetLinha();
	int coluna2 = c2.GetColuna();

	if((linha1 > linha2) && (coluna1 > coluna2))
	{
		return SW;
	}
	else if((linha1<linha2) && (coluna1<coluna2))
	{
		return NE;
	}
	else if((linha1 < linha2) &&  (coluna1>coluna2))
	{
		return NW;
	}
	else if((linha1 > linha2) && (coluna1<coluna2))
	{
		return SE;
	}
	else
	{
		return ERRO;
	}
}

//Verifica se a peça se encontra dentro das margens do tabuleiro
bool Tabuleiro::ValidaMargens(int linha, int coluna, int dist)
{
	if((linha+dist < 8) && (linha-dist >= 0) && (coluna+dist < 8) && (coluna-dist>= 0))
	{
		return true;
	}
	return false;
}

//Verifica se pode comer alguma peça e devolver um signal de erro
Tabuleiro::erro Tabuleiro::PodeComerDama(Casa from, Casa to, diagonalDirection dir)
{
	int linha = from.GetLinha();
	int coluna = from.GetColuna();
	int counter = 0;
	
	while((linha != to.GetLinha()) && (coluna != to.GetColuna()))
	{
		AvancaDiagonal(&linha, &coluna, dir);
		if(this->casas[linha][coluna]->GetContent() != NULL)
		{
			counter ++;
		}
	}
	if(counter == 1)
	{
		return OK;
	}
	else if(counter > 1)
	{
		return BLOCKED;
	}
	else
	{
		return OTHER;
	}
}

// Função para avaliar se um peão pode comer
Tabuleiro::erro Tabuleiro::PodeComer(Casa from, Casa to, diagonalDirection dir )
{
	int linha = from.GetLinha();
	int coluna = from.GetColuna();
	AvancaDiagonal(&linha, &coluna, dir);

	if(( to.GetContent() == NULL) && (from.GetContent() != NULL))
	{
		if(this->casas[linha][coluna]->GetContent() != NULL)
		{
			if(this->casas[linha][coluna]->GetContent()->GetCor() != from.GetContent()->GetCor())
			{
				return OK;
			}
			else
			{
				return BLOCKED;
			}
		}
		else
		{
			return OTHER;
		}
	}
	else if((( to.GetContent() != NULL) && (this->casas[linha][coluna]->GetContent() != NULL)))
	{
		return BLOCKED;
	}
	else if(!ValidaMargens(linha, coluna, 0))
	{
		return OFFBOARD;	
	}
	else
	{
		return OTHER;
	}
}

//Verifica se existem jogadas obrigatorias
bool Tabuleiro::JogadaObrigatoria(Casa* from)
{
	bool flag = false;
	std::vector<std::vector<Caminho>> caminhosobrigatorios;
	caminhosobrigatorios.clear();
	if(from->GetContent() != NULL)
	{
		Dama* d = from->GetContent();
		if((d->GetTipo() == Dama::PEAO) && (d->GetCor()== Dama::PRETO))
		{
			Caminho path = Caminho();
			path.SetDestino(from);
			JogadaObrigatoriaPeao(from, this->SW, &path);
			JogadaObrigatoriaPeao(from, this->SE, &path);
			std::vector<Caminho> vecPath;
			path.PathValue(&path, vecPath, &caminhosobrigatorios);
		}
		if((d->GetTipo() == Dama::PEAO) && (d->GetCor()== Dama::BRANCO))
		{
			Caminho path = Caminho();
			path.SetDestino(from);
			JogadaObrigatoriaPeao(from, this->NW, &path);
			JogadaObrigatoriaPeao(from, this->NE, &path);
			std::vector<Caminho> vecPath;
			path.PathValue(&path, vecPath, &caminhosobrigatorios);
		}
		if(d->GetTipo() == Dama::DAMA)
		{
			Caminho path = Caminho();
			path.SetDestino(from);
			JogadaObrigatoriaDama(from, this->NW, &path);
			JogadaObrigatoriaDama(from, this->NE, &path);
			JogadaObrigatoriaDama(from, this->SE, &path);
			JogadaObrigatoriaDama(from, this->SW, &path);
			std::vector<Caminho> vecPath;
			path.PathValue(&path, vecPath, &caminhosobrigatorios);
		}
		int max=0, imax = 0;

		// Avalia melhor caminho
		for(int i = 0; i < (int)caminhosobrigatorios.size(); i++)
		{
			int sum=0;
			std::vector<Caminho> jogada = caminhosobrigatorios[i];
			for(int j = 0; j < (int)jogada.size(); j++)
			{
				Caminho c = jogada[j];
				sum += c.GetValor() * (j+1);
			}
			if (sum > max)
			{
				max = sum;
			}
		}
		
		// Grava os melhores caminhos
		for(int i = 0; i < (int)caminhosobrigatorios.size(); i++)
		{
			int sum = 0;
			std::vector<Caminho> jogada = caminhosobrigatorios[i];
			for(int j = 0; j < (int)jogada.size(); j++)
			{
				Caminho c = jogada[j];
				sum += c.GetValor() * (j+1);
			}
			if (sum == max)
			{
				if (sum > 0)
				{
					jogadasobrigatorias.push_back(jogada);
					flag = true;
				}
			}
		}
	}
	return flag;
}

//Verifica jogada obrigatoria de uma dama
void Tabuleiro::JogadaObrigatoriaDama(Casa* from, diagonalDirection dir, Caminho* path, Dama* dama)
{
	int coluna = from->GetColuna();
	int linha = from->GetLinha();
	int colunasum = coluna;
	int linhasum = linha;
	Dama* d1;
	if (dama != NULL)
	{
		d1 = dama;
	}
	else if(from->GetContent() != NULL)
	{
		d1 = from->GetContent();
	}
	if(d1 != NULL)
	{
		bool find1 = true;
		while(find1)
		{
			if(ValidaMargens(linhasum, colunasum, 0))
			{
				erro e = PodeComerDama(*from, *this->casas[linhasum][colunasum], dir);
				if( e == OK) 
				{
					find1= false;
						Dama* d2 = this->casas[linhasum][colunasum]->GetContent();
						if (d1->GetCor() != d2->GetCor()) 
						{
							int valor =0;
							if(d2->GetTipo() == Dama::PEAO)
							{
								valor = 1;
							}
							else
							{
								valor = 2;
							}	
							AvancaDiagonal(&linhasum, &colunasum, dir);
							if(ValidaMargens(linhasum, colunasum,0))
							{
								if(this->casas[linhasum][colunasum]->GetContent() == NULL)
								{
									Caminho* psw = new Caminho();
									Caminho* pse = new Caminho();
									Caminho* pnw = new Caminho();
									Caminho* pne = new Caminho();
									switch(dir)
									{
										case SW:
											
											psw->SetOrigem(from);
											psw->SetDestino(this->casas[linhasum][colunasum]);
											psw->SetValor(valor);
											path->SetSW(psw);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], NW, path->GetSW(), d1);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], SE, path->GetSW(), d1);
											break;
										case SE:
											
											pse->SetOrigem(from);
											pse->SetDestino(this->casas[linhasum][colunasum]);
											pse->SetValor(valor);
											path->SetSE(pse);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], SW, path->GetSE(), d1);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], NE, path->GetSE(), d1);
											break;
										case NW:
											
											pnw->SetOrigem(from);
											pnw->SetDestino(this->casas[linhasum][colunasum]);
											pnw->SetValor(valor);
											path->SetNW(pnw);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], SW, path->GetNW(), d1);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], NE, path->GetNW(), d1);
											break;
										case NE:
											
											pne->SetOrigem(from);
											pne->SetDestino(this->casas[linhasum][colunasum]);
											pne->SetValor(valor);
											path->SetNE(pne);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], NW, path->GetNE(), d1);
											Tabuleiro::JogadaObrigatoriaDama(this->casas[linhasum][colunasum], SE, path->GetNE(), d1);
											break;
									}
								
								}
							}
							else
							{
								e = BLOCKED;
							}
						}

				}
				else
				{
					AvancaDiagonal(&linhasum, &colunasum, dir);
					if((e == BLOCKED )|| (e == OFFBOARD))
					{
						find1=  false;
					}
				}
			}
			else
			{
				find1 = false;
			}
		}
	}
}

//verifica jogada obrigatoria de um peao
void Tabuleiro::JogadaObrigatoriaPeao(Casa* from, diagonalDirection dir, Caminho* path)
{
	int coluna = from->GetColuna();
	int linha = from->GetLinha();
	int colunasum = coluna;
	int linhasum = linha;
	
	AvancaDiagonal(&linhasum, &colunasum, dir);
	if(from->GetContent() != NULL)
	{
		Dama* d1 = from->GetContent();
		if(ValidaMargens(linhasum, colunasum,0))
		{
			if(this->casas[linhasum][colunasum]->GetContent() != NULL)
			{
				Dama* d2 = this->casas[linhasum][colunasum]->GetContent();
				AvancaDiagonal(&linhasum, &colunasum, dir);
				if(ValidaMargens(linhasum,colunasum,0))
				{
					if (this->casas[linhasum][colunasum]->GetContent() == NULL)
					{
						if(d1->GetCor() != d2->GetCor()) 
						{
							int valor =0;
							if(d2->GetTipo() == Dama::PEAO)
							{	
								valor = 1;
							}
							else
							{
								valor = 2;
							}	
							Caminho* pse = new Caminho();
							Caminho* psw = new Caminho();
							Caminho* pne = new Caminho();
							Caminho* pnw = new Caminho();
							switch(dir)
							{
								case SE:
									
									pse->SetValor(valor);
									pse->SetOrigem(from);
									pse->SetDestino(this->casas[linhasum][colunasum]);
									path->SetSE(pse);
									break;
								case SW:
									
									psw->SetValor(valor);
									psw->SetOrigem(from);
									psw->SetDestino(this->casas[linhasum][colunasum]);
									path->SetSW(psw);
									break;
								case NE:
									
									pne->SetValor(valor);
									pne->SetOrigem(from);
									pne->SetDestino(this->casas[linhasum][colunasum]);
									path->SetNE(pne);
									break;
								case NW:
									
									pnw->SetValor(valor);
									pnw->SetOrigem(from);
									pnw->SetDestino(this->casas[linhasum][colunasum]);
									path->SetNW(pnw);
									break;
							}
						}
					}
				}
			}
		}
	}
}

// Metodo para comer peças
void Tabuleiro::ComePeca(Casa* from, Casa* to)
{
	diagonalDirection dir = DescobreDireccao(*from, *to);
	int linha = from->GetLinha();
	int coluna = from->GetColuna();
	AvancaDiagonal(&linha,&coluna, dir);
	while((to->GetLinha() != linha) && (to->GetColuna() != coluna))
	{
		if(this->casas[linha][coluna]->GetContent() != NULL)
		{
			this->casas[linha][coluna]->SetContent(NULL);
		}
		AvancaDiagonal(&linha,&coluna, dir);
	}
}

//Metodo para movimentar uma peça
Tabuleiro::erro Tabuleiro::Move(Casa* from, Casa* to)
{
	int distance = from->DistanceTo(to);
	Dama* d1 = from->GetContent();
	if(from->isDiagonal(to))
	{
		if (d1->GetTipo() == Dama::PEAO)
		{
			if(d1->GetCor() == Dama::BRANCO)
			{
				if(distance <= 0)
				{
					return ILLEGALMOVE;
				}
			}
			else
			{
				if(distance >= 0)
				{
					return ILLEGALMOVE;
				}
			}
				
			if(abs(distance) == 1)
			{
				if(ValidaMargens(to->GetLinha(), to->GetColuna(),0))
				{
					
					if(to->GetContent() == NULL)
					{
						from->SetContent(NULL);
						to->SetContent(d1);
					}
				}
			}
			else if(abs(distance) == 2)
			{
				//come peça
				if(ValidaMargens(to->GetLinha(), to->GetColuna(),0))
				{
					if(to->GetContent() == NULL)
					{
						diagonalDirection dir = DescobreDireccao(*from, *to);
						if (this->PodeComer(*from, *to, dir) == Tabuleiro::OK)
						{
							ComePeca(from, to);
							from->SetContent(NULL);
							to->SetContent(d1);
						}
					}
				}
			}
			else
			{
				return ILLEGALMOVE;
			}
		}
		else
		{
			int linha1 = from->GetLinha();
			int linha2 = to->GetLinha();
			int coluna1 = from->GetColuna();
			int coluna2 = to->GetColuna();

			diagonalDirection dir = DescobreDireccao(*from, *to);
			Tabuleiro::erro err = PodeComerDama(*from, *to, dir);
			if (err == OK)
			{
				ComePeca(from, to);
				if(to->GetContent() == NULL)
				{
					from->SetContent(NULL);
					to->SetContent(d1);
				}
			}
			else
			{
				int counter = 0;
				for(int i = 0; i < distance; i++)
				{
					AvancaDiagonal(&linha1, &coluna1, dir);
					if(this->casas[linha1][coluna1]->GetContent() != NULL)
					{
						counter++;
					}
				}
				if(counter == 0)
				{
					if(to->GetContent() == NULL)
					{
						from->SetContent(NULL);
						to->SetContent(d1);
						err = Tabuleiro::OK;
					}
					else
					{
						err= Tabuleiro::ILLEGALMOVE;
					}
				}
				else
				{
					err = Tabuleiro::ILLEGALMOVE;
				}
			}
			return err;
		}

	}
	return OK;
}

// Metodo para obter jogadas obrigatórias
std::vector<std::vector<Jogada>> Tabuleiro::JogadasObrigatorias()
{
	for(int i = 0; i < (int)this->jogadasobrigatorias.size(); i++)
	{
		std::vector<Jogada> aux;
		for(int j =0; j < (int)this->jogadasobrigatorias[i].size(); j++)
		{
			Jogada jogaux = Jogada(this->jogadasobrigatorias[i][j].GetOrigem()->GetLinha(),
									this->jogadasobrigatorias[i][j].GetOrigem()->GetColuna(),
									this->jogadasobrigatorias[i][j].GetDestino()->GetLinha(),
									this->jogadasobrigatorias[i][j].GetDestino()->GetColuna());
			aux.push_back(jogaux);
		}
		this->jog.push_back(aux);
	}
	std::vector<std::vector<Jogada>> ret = this->jog;
	this->jog.clear();
	this->jogadasobrigatorias.clear();
	return ret;
}

///Metodo auxiliar para validar jogadas
bool Tabuleiro::DamaPodeContinuar(Casa* casa, diagonalDirection dir, Dama* d)
{
	Caminho* testa1= new Caminho();
	Caminho* testa2= new Caminho();
	switch (dir)
	{
		case Tabuleiro::SE:
			this->JogadaObrigatoriaDama(casa, Tabuleiro::SW, testa1,d);
			this->JogadaObrigatoriaDama(casa, Tabuleiro::NE, testa2,d);
			
			break;
		case Tabuleiro::SW:
			this->JogadaObrigatoriaDama(casa, Tabuleiro::NW, testa1,d);
			this->JogadaObrigatoriaDama(casa, Tabuleiro::SE, testa2,d);
			
			break;
		case Tabuleiro::NE:
			this->JogadaObrigatoriaDama(casa, Tabuleiro::NW, testa1,d);
			this->JogadaObrigatoriaDama(casa, Tabuleiro::SE, testa2,d);
			
			break;
		case Tabuleiro::NW:
			this->JogadaObrigatoriaDama(casa, Tabuleiro::SW, testa1,d);
			this->JogadaObrigatoriaDama(casa, Tabuleiro::NE, testa2,d);
			
			break;
	}

	if((testa1->GetNE() != NULL) || (testa1->GetNW() != NULL) || (testa1->GetSE() != NULL) || (testa1->GetSW() != NULL))
	{
		return true;
	
	}
	if((testa2->GetNE() != NULL) || (testa2->GetNW() != NULL) || (testa2->GetSE() != NULL) || (testa2->GetSW() != NULL))
	{
		return true;
	}
	return false;

}

///Metodo para selecionar casa
void Tabuleiro::SelecionaCasa(int linha, int coluna)
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			this->casas[i][j]->Unselect();
		}
	}
	this->casas[linha][coluna]->Select();
}