#include "dama.h"
#include "stdafx.h"

Dama::Dama(void)
{
}

Dama::Dama(	cor c )
{
	this->cordama = c;
	this->tipodama = PEAO;
}

Dama::Dama(cor c, tipo t)
{
	this->cordama = c;
	this->tipodama = t;
}


Dama::~Dama(void)
{
}

void Dama::Promote()
{
	this->tipodama = DAMA;
}

Dama::cor Dama::GetCor()
{
	return this->cordama;
}

Dama::tipo Dama::GetTipo()
{
	return this->tipodama;
}