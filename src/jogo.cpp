#include "jogo.h"
#include <vector>
#include "stdafx.h"
#include <sstream>
#include <iomanip>
#include "caminho.h"

Jogo::Jogo(void)
{
	
}

///Variaveis estaticas
int Jogo::numerojogada;
Jogo::jogador Jogo::jogadoractivo;
int Jogo::movimento;
Jogo::vitoria Jogo::vit;

///Inicializa o tabuleiro
void Jogo::InitTabuleiro()
{
	this->tab = new Tabuleiro();
	SetDamasBrancas();
	SetDamasPretas();

	//Dados de teste
	//this->tab->casas[1][4]->SetContent(new Dama(Dama::BRANCO, Dama::DAMA));
	//this->tab->casas[2][3]->SetContent(new Dama(Dama::PRETO, Dama::PEAO));
	//this->tab->casas[2][5]->SetContent(new Dama(Dama::PRETO, Dama::PEAO));
	//this->tab->casas[4][3]->SetContent(new Dama(Dama::PRETO, Dama::PEAO));
	//this->tab->casas[4][7]->SetContent(new Dama(Dama::PRETO, Dama::PEAO));
	numerojogada = 0;
	movimento =0;
	jogadoractivo = JOGADOR1;
}

///Dispões Damas Pretas no tabuleiro
void Jogo::SetDamasPretas()
{
	for(int i = 7; i > 4; i--)
	{
		for(int j = 0; j < 8; j++)
		{
			if((i+j)%2 == 1)
			{
				this->tab->casas[i][j]->SetContent(new Dama(Dama::PRETO,Dama::PEAO));
			}
		}
	}
}

///Dispõe Damas Brancas no tabuleiro
void Jogo::SetDamasBrancas()
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if((i+j)%2 == 1)
			{
				this->tab->casas[i][j]->SetContent(new Dama(Dama::BRANCO, Dama::PEAO));
			}
		}
	}
}

/// Avalia se existem jogadas obrigatórias
bool Jogo::AvaliaJogadasObrigatorias(Dama::cor c)
{
	bool flag = false;	
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j <8; j++)
		{
			if(this->tab->casas[i][j]->GetContent() != NULL)
			{
				Dama d = *this->tab->casas[i][j]->GetContent();
				if(d.GetCor() == c)
				{
					if(this->tab->JogadaObrigatoria(this->tab->casas[i][j]))
					{
						flag = true;
					}
				}
			}
		}
	}
	return flag;
}

void Jogo::VerificaVitoria()
{
	if(this->contador == 30)
	{
		vit = EMPATE;//empate
	}
	int brancas=0;
	int pretas = 0;
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(this->tab->casas[i][j]->GetContent() != NULL)
			{
				if(this->tab->casas[i][j]->GetContent()->GetCor() == Dama::PRETO)
				{
					pretas++;
				}
				else
				{
					brancas++;
				}
			}
		}
	}
	if(pretas == 0)
	{
		vit = JOGPRETA;//branco ganha
	}
	else if(brancas == 0)
	{
		vit= JOGBRANCA;//preto ganha
	}

}

/// Metodo para avançar turno
void Jogo::Turno()
{
	VerificaVitoria();
	AvaliaPromocao();
	numerojogada++;
	if (jogadoractivo == JOGADOR1)
	{
		jogadoractivo = JOGADOR2;
	}
	else
	{
		jogadoractivo = JOGADOR1;
	}
	jobrigatorias.clear();
	movimento = 0;
	
}

///Metodo para forçar regra da prioridade de captura
bool Jogo::ObrigadoComer()
{
	if(jogadoractivo == JOGADOR1)
	{
		return AvaliaJogadasObrigatorias(Dama::BRANCO);
	}
	else
	{
		return AvaliaJogadasObrigatorias(Dama::PRETO);
	}
	return true;
}

void Jogo::AvaliaPromocao()
{
	for(int i = 0; i < 8; i ++)
	{
		if(this->tab->casas[0][i]->GetContent() != NULL)
		{
			if(this->tab->casas[0][i]->GetContent()->GetCor() == Dama::PRETO)
			{
				this->tab->casas[0][i]->GetContent()->Promote();
			}
		}
		else if(this->tab->casas[7][i]->GetContent() != NULL)
		{
			if(this->tab->casas[7][i]->GetContent()->GetCor() == Dama::BRANCO)
			{
				this->tab->casas[7][i]->GetContent()->Promote();
			}
		}
	}
}

void Jogo::FazJogada(Jogada j)
{
	//primeiro verificamos se é obrigado a comer
	if(this->ObrigadoComer())
	{	
		//reset ao contador de jogadas sem comer
		this->contador = 0;
		bool finish= false;
		std::vector<std::vector<Jogada>> jogadas ;
		
		//se o contador de movimentos num turno for zero precisamos de obter lista de jogadas obrigatorias
		if(movimento == 0)
		{
			this->obrig = this->tab->JogadasObrigatorias();
		}

		jogadas = this->obrig;
		this->jobrigatorias.push_back(j);
		bool flag= false;
		int fromX = j.GetOrigem().GetX();
		int fromY = j.GetOrigem().GetY();
		int toX = j.GetDestino().GetX();
		int toY = j.GetDestino().GetY();
		
		
		//Para cada caminho das jogadas obrigatorias vamos verificar
		for(int i = 0; i < (int)jogadas.size(); i++)
		{
			//Se for uma dama tem regras especias
			if(this->tab->casas[fromX][fromY]->GetContent()->GetTipo() == Dama::DAMA )
			{

				if((int)jogadas[i].size() > movimento)
				{
					if(jogadas[i][movimento].Compara(jobrigatorias[movimento]))
					{
						//Se o destino for igual à casa de destino esperada 
						flag = true;
						if(movimento == jogadas[i].size()-1)
						{
							finish = true;
						}
					}
					else
					{
						//De outra forma temos de reavaliar a situaação de jogo
						Casa casa1 = Casa(j.GetOrigem().GetX(), j.GetOrigem().GetY());
						Casa casa2 = Casa(j.GetDestino().GetX(), j.GetDestino().GetY());
						Casa casa3 = Casa(jogadas[i][movimento].GetDestino().GetX(),jogadas[i][movimento].GetDestino().GetY());
						Dama d = *this->tab->casas[fromX][fromY]->GetContent();
						int distance = abs(casa1.DistanceTo(&casa2));
						Tabuleiro::diagonalDirection dir = this->tab->DescobreDireccao(Casa(j.GetOrigem().GetX(), j.GetOrigem().GetY()), casa2);
						
						bool continua = this->tab->DamaPodeContinuar(&casa2, dir, &d);
						bool podiaContinuar = false;
						int linha = j.GetOrigem().GetX();
						int coluna = j.GetOrigem().GetY();
						int counter = 0;
						int damaerro = 0;
						for(int k = 0; k < distance; k++)
						{
							this->tab->AvancaDiagonal(&linha, &coluna, dir);
							if (this->tab->DamaPodeContinuar(new Casa(linha,coluna), dir, &d))
							{
								podiaContinuar = true;
							}
							
								if(this->tab->casas[linha][coluna]->GetContent() != NULL)
								{
									if(this->tab->casas[linha][coluna]->GetContent()->GetCor() != d.GetCor())
									{
										counter++;
									}
									else
									{
										damaerro++;
									}
							}
							
						}
						if(damaerro == 0)
						{
							if (counter ==1)
							{	
								if(continua)
								{
									this->obrig = this->tab->JogadasObrigatorias();
									jobrigatorias.clear();
									flag = true;
									movimento--;
									finish = false;
								}
								else
								{
									if(podiaContinuar)
									{
										flag = false;
									}
									else
									{				
										this->obrig = this->tab->JogadasObrigatorias();
										jobrigatorias.clear();
										flag = true;
										movimento--;
										finish = true;

									}
								}
							}
						}
						else
						{
							flag = false;
						}
					}
				}
			}
			else
			{
				if(jogadas[i][movimento].Compara(jobrigatorias[movimento]))
				{
					flag = true;
					if(movimento == jogadas[i].size()-1)
					{
						finish = true;
					}
				}
			}
			
		}
		if(flag)
		{
			this->tab->Move(this->tab->casas[fromX][fromY], this->tab->casas[toX][toY]);
			movimento++;
			if(finish)
			{
				Erro = Tabuleiro::OK;
				Turno();
			}
		}
		else
		{
			Erro = Tabuleiro::MANDATORYMOVE;
			jobrigatorias.pop_back();
		}
	}
	else
	{
		Erro = (int)this->tab->Move(this->tab->casas[j.GetOrigem().GetX()][j.GetOrigem().GetY()], this->tab->casas[j.GetDestino().GetX()][j.GetDestino().GetY()]);
		if (Erro == Tabuleiro::OK)
		{
			Turno();
		}
	}
}

std::string Jogo::GetError()
{
	std::string mensagem ="";
	std::ostringstream convert;
	if(Erro == Tabuleiro::BLOCKED)
	{
		convert << "jogada Bloqueada";
	}
	else if(Erro == Tabuleiro::OTHER)
	{
		convert << "Ocorreu um erro";
	}
	else if(Erro == Tabuleiro::ILLEGALMOVE)
	{
		convert << "Jogada Ilegal";
	}
	else if(Erro == Tabuleiro::MANDATORYMOVE)
	{
		convert << "Jogada Obrigatória";
	}
	mensagem = convert.str();
	return mensagem;
}

std::string Jogo::GetStatus()
{
	std::string mensagem ="Turno: ";
	std::ostringstream convert;

	convert << "Turno: " << this->numerojogada << " - " << "Jogador: " << (this->jogadoractivo == 0 ? "Brancas " : "Pretas ") << "\r\n";
	
	mensagem = convert.str();
	this->status = mensagem;
	return  this->status;
}

Jogo::~Jogo(void)
{
}
