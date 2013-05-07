#pragma once


struct Dama
{
	
	enum cor{BRANCO = 1, PRETO};
	enum tipo {PEAO = 1, DAMA};
	
	Dama(void);
	Dama(cor c);
	Dama(cor c, tipo t);
	
	~Dama(void);

	void Promote();
	tipo GetTipo();
	cor GetCor();

	private:
	Dama::cor cordama;
	Dama::tipo tipodama;
};

