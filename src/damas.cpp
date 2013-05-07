#include <windows.h>

#include "stdafx.h"
#include <vector>
#include <stdlib.h>

#include "jogo.h"
#include "tabuleiro.h"
#include "casa.h"
#include "dama.h"
#include "cordenada.h"
#include <string>


//const char g_szClassName[] = "myWindowClass";
POINT p1;
POINT p2;
Cordenada origem; 
Cordenada destino;
Jogo* game = new Jogo();
static HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
static HPEN hPenSel = CreatePen(PS_SOLID, 3, RGB(255,0,0));
static HBRUSH hBrushgrey =CreateSolidBrush(RGB(200,200,200));
static HBRUSH hBrushblack =CreateSolidBrush(RGB(255,255,255));
static HBRUSH hBrushwhite = CreateSolidBrush(RGB(0,0,0));


void ValidateOrigin(Cordenada c)
{
	
	int x = c.GetX();
	int y = c.GetY();
	if ((x>=0) && (y>=0))
	{
		if(game->tab->casas[x][y]->GetContent() != NULL)
		{
			Dama* d = game->tab->casas[x][y]->GetContent();
			if(game->jogadoractivo == 0)
			{			
				if (d->GetCor() == Dama::BRANCO)
				{
					origem.Set(c.GetX(), c.GetY());
				}
				else
				{
					origem.Set(-1,-1);
				}
			}	
			else
			{
				if (d->GetCor() == Dama::PRETO)
				{
					origem.Set(c.GetX(), c.GetY());
				}
				else
				{
					origem.Set(-1,-1);
				}
			}
		}
	}
}

Cordenada GetCasaFromCord(int x, int y, HWND hwnd)
{
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	Cordenada result = Cordenada(-1,-1);
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			int width = (rcClient.right-200 - rcClient.left) / 8;
			int height = (rcClient.bottom - rcClient.top) / 8;
			
			RECT r;
			r.left = width*j;
			r.top = rcClient.bottom - (height * i);
			r.right = width*(j+1);
			r.bottom = rcClient.bottom - height * (i+1);
			if((i+j) % 2 == 1)
			{
				if((x >= r.left) && (x < r.right) && (y < r.top) && (y >= r.bottom))
				{
					result = Cordenada(i,j);
				}
			}
		}
	}
	return result;
}

void ValidateDestination(Cordenada c)
{
	int x = c.GetX();
	int y = c.GetY();
	if ((x>=0) && (y>=0))
	{
		if(game->tab->casas[x][y]->GetContent() != NULL)
		{
			ValidateOrigin(c);
			return;
		}
		else
		{
			destino.Set(c.GetX(), c.GetY());
		}
	}
}

void TabuleiroCasas(HDC hdc, HWND hwnd, RECT rcClient)
{
	PAINTSTRUCT Ps;
	hdc = BeginPaint(hwnd, &Ps);
	SelectObject(hdc, hBrushgrey);
	Rectangle(hdc, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
	Rectangle(hdc, rcClient.left, rcClient.top, rcClient.right-200, rcClient.bottom);
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			int width = (rcClient.right-200 - rcClient.left) / 8;
			int height = (rcClient.bottom - rcClient.top) / 8;
			
			TextOutA(hdc, rcClient.right -200 , 10, game->GetStatus().c_str(), (int)game->GetStatus().size());
			TextOutA(hdc, rcClient.right -200 , 30, game->GetError().c_str(), (int) game->GetError().size());
			
			RECT r;
			r.left = width*j;
			r.top = rcClient.bottom - (height * i);
			r.right = width*(j+1);
			r.bottom = rcClient.bottom - height * (i+1);
			
			if((i+j) % 2 == 1)
			{
				if((p1.x >= r.left) && (p1.x < r.right) && (p1.y < r.top) && (p1.y >= r.bottom))
				{
					SelectObject(hdc, hPenSel);
				}
				else
				{
					SelectObject(hdc, hPen);
				}
				SelectObject(hdc, hBrushwhite);
				Rectangle(hdc, r.left ,r.bottom, r.right, r.top);
				if(game->tab->casas[i][j]->GetContent() != NULL)
				{
					Dama* d = game->tab->casas[i][j]->GetContent();
					if (d->GetCor() == Dama::PRETO)
					{
						SelectObject(hdc, hBrushgrey);
						Ellipse(hdc,r.left, r.bottom, r.right, r.top);
						if(d->GetTipo() == Dama::DAMA)
						{
							Ellipse(hdc, r.left+5, r.bottom+5, r.right-5, r.top-5);
							Ellipse(hdc, r.left+10, r.bottom+10, r.right-10, r.top-10);
							Ellipse(hdc, r.left+15, r.bottom+15, r.right-15, r.top-15);
						}
					}
					else
					{
						SelectObject(hdc, hBrushblack);
						Ellipse(hdc,r.left, r.bottom, r.right, r.top);
						if(d->GetTipo() == Dama::DAMA)
						{
							Ellipse(hdc, r.left+5, r.bottom+5, r.right-5, r.top-5);
							Ellipse(hdc, r.left+10, r.bottom+10, r.right-10, r.top-10);
							Ellipse(hdc, r.left+15, r.bottom+15, r.right-15, r.top-15);
						}
					}
				}
			}
			else
			{
				SelectObject(hdc, hPen);
				SelectObject(hdc, hBrushblack);
				Rectangle(hdc, r.left ,r.bottom,r.right, r.top);
			}
		}
	}

	LPRECT rect = LPRECT(&rcClient);
		rect->bottom = rcClient.bottom;
		rect->left = rcClient.left;
		rect->right = rcClient.right;
		rect->top = rcClient.top;
	if(game->vit == Jogo::JOGBRANCA)
	{
		
		DrawText(hdc, L"JOGADOR BRANCO GANHOU", 22, rect, DT_CENTER);
	}
	else if(game->vit == Jogo::JOGPRETA)
	{
		DrawText(hdc, L"JOGADOR BRANCO GANHOU",22, rect, DT_CENTER);
	}
	
	EndPaint(hwnd, &Ps);
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	std::vector<RECT> casas;
	Casa c[8][8];
	int xPos; 
	int yPos;
    switch(msg)
    {
		case WM_LBUTTONDOWN:
			if (origem.GetX() == -1)
			{
				p1.x = GET_X_LPARAM(lParam); 
				p1.y = GET_Y_LPARAM(lParam);

				ValidateOrigin(GetCasaFromCord(p1.x, p1.y, hwnd));
			}
			else if((origem.GetX() > -1) && (destino.GetX() == -1))
			{
				p1.x = GET_X_LPARAM(lParam);
				p1.y = GET_Y_LPARAM(lParam);
				ValidateDestination(GetCasaFromCord(p1.x, p1.y, hwnd));
				p1.x = 0;
				p1.y = 0;
				p2.x = 0;
				p2.y = 0;
			}
		InvalidateRect(hwnd, NULL, true);
		
		break;
		case WM_LBUTTONDBLCLK:
			
			break;
		case WM_PAINT:
			
			RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			HDC hdc;
			
			TabuleiroCasas(hdc, hwnd, rcClient);

		break;
		case WM_SIZE:
			InvalidateRect(hwnd, NULL, false);
			break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"Damas";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        3,
        L"Damas",
        L"E-Folio A - Jogo das Damas",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
	p1.x = 0;
	p1.y = 0;
	p2.x = 0;
	p2.y = 0;
	origem = Cordenada(-1,-1);
	destino= Cordenada(-1,-1);
	game->InitTabuleiro();
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
		//Damas logic
		if((origem.GetX() != -1) && (destino.GetY() != -1))
		{
			//Move Done
			game->FazJogada(Jogada(origem.GetX(), origem.GetY(), destino.GetX(), destino.GetY()));
			InvalidateRect(hwnd, NULL, true);
			origem.Set(-1,-1);
			destino.Set(-1,-1);
		}


        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

