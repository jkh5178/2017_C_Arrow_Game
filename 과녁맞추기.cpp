// ������߱�.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "������߱�.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      80, 100, 1800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd,nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//

#define AROWSIZE 100	//��ǥ�� ũ��
#define MAXPOWER 300	//�ִ�power
#define BALLSIZE 15		//��ź ������
#define PI 3.141592		//PI ��
#define MOVEOBJECT 5	//������ �� ���� ������ �ȼ�
#define MOVEPOWER 20	//power�� �� ���� ��ȭ��ų ��
#define MOVECANNONBALL 45	//������ ��� ������ x ��ǥ

//��Ʈ�� ��� �Լ�
void DrawBitmap(HDC hdc, int x, int y, int sizex, int sizey, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit; 

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	StretchBlt(hdc, x, y, sizex, sizey, MemDC, 0, 0, bx, by, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

//ǥ�� �� power������ �̵� ���� ����
int Move( bool &b, int num) {
	switch (b){
	case true: {
		return +num;
	}
	case false: {
		return -num;
	}
	}
}

//��ǥ�� �׸���(���콺 ��ǥ�� ���� ���Ѵ�)
void drawarrow(HDC hdc, int x, int y,int setx, int sety) {
	int LineX, LineY;
	LineX =  setx- AROWSIZE*(setx - x) / sqrt(pow(setx - x, 2) + pow(sety - y, 2));
	LineY=   sety- AROWSIZE*(sety - y) / sqrt(pow(setx - x, 2) + pow(sety - y, 2));
	MoveToEx(hdc, setx, sety,NULL);
	LineTo(hdc,LineX, LineY);

}
//�������
float getdgree(int mousex, int mousey, int initx, int inity) {

	int a = abs(initx - mousex);
	int b = abs(inity - mousey);
	float c = sqrt(pow(a, 2) + pow(b, 2));

	float dgree = acosf(a / c);
	if (inity > mousey) {
		dgree = 2 * PI - dgree;
	}
	return dgree;
}

POINT shutpoint(int power, float x, float dgree) {
	POINT pt;
	float t = x /( abs(power)/1.7*cos(2 * PI - dgree));
	pt.x = x;
	pt.y = (x*(sin(2 * PI - dgree)))/cos(2*PI-dgree) - (9.8*t*t)/2;
	
	return pt;


}
//ȭ�� ���� ��������
bool checkball(int ballptx, int ballpty,int endx,int endy) {
	if (ballptx > endx || ballpty > endy-1) {
		return true;
	}
	else
		return false;
}
//����� ��������
bool meetobject(int objectx, int objecty, int objectsizex, int objectsizey, int ballx, int bally) {
	if ((ballx >= objectx) ) {
		if ((bally >= objecty+10) && (bally <= (objecty-10+ objectsizey))) {
			return true;
		}
		else
			return false;
	}
	else 
		return false;
}
//levelüũ
int checklevel(int score) {
	if (score >= 45) {
		return 4;
	}
	else if (score >= 20) {
		return 3;
	}
	else if (score >= 6) {
		return 2;
	}
	else
		return 1;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static bool bUp = false, bPower=false, bMouse=true;
	static HBITMAP Backgraund,MyBitmap1[2], MyBitmap2[2];
	static RECT rtall,//â��ü�� ũ��
		rtboards, rtboardl,//�������� ũ��
		rtarrow;
	static int objectset, //������ ��ġ
			arrowpointx=0, arrowpointy=0, //��ǥ���� ��ǥ��
			power=-300,score=0,level=1;
	static POINT pt1, ballpt2 = { 0,0 },//���� ������ �Ÿ�
					mousept;//���콺 ��ǥ��
	static float time = 0,dgree=0;
	static TCHAR cscore[128], clevel[128];

	switch (message)
	{
	case WM_CREATE: {
		Backgraund = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREE));//���� ���ȭ��
		MyBitmap1[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_OBJECT));//���� ���ȭ��
		MyBitmap1[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOARD));//���� ���ȭ��
		MyBitmap2[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWERBACK));//�Ŀ�������
		MyBitmap2[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWER));//�Ŀ�������

		GetClientRect(hWnd, &rtall);
		rtboards = { (rtall.right / 2) - 88, 100, ((rtall.right / 2) - 88 + 220 / 2), 150 };
		rtboardl = { ((rtall.right / 2) - 88 + 220 / 2), 100 , ((rtall.right / 2) - 88 + 220), 150 };
		rtarrow = { rtall.bottom - AROWSIZE, rtall.left, rtall.bottom, rtall.left + AROWSIZE };
		objectset = (rtall.bottom / 2)-200;
		SetTimer(hWnd, 1, 80, NULL);
		pt1 = { rtall.left, rtall.bottom };// â�� ũ��
		break; }
	case WM_COMMAND:
	
	case WM_TIMER: {
		switch (wParam)
		{
		case 1: {
			if (objectset + 200 >= rtall.bottom) { bUp = false; }
			else if (objectset <= rtall.top) { bUp = true; }
			objectset = objectset + Move(bUp, MOVEOBJECT);
			InvalidateRect(hWnd, NULL, true);
			break; }
		case 2: {
			if (-power >=MAXPOWER ) { bPower = false; }
			else if (-power <= 0) { bPower =true; }
			power = power - Move(bPower, MOVEPOWER);
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		case 3: {
			ballpt2.x = ballpt2.x + MOVECANNONBALL;
			ballpt2 = shutpoint(power, ballpt2.x, dgree);

			if (meetobject(rtall.right - 50, objectset, 50, 200, pt1.x + ballpt2.x, pt1.y-ballpt2.y)) {
				score = score + (2 * level);
				level=checklevel(score);
				//�ʱ�ȭ ����
				KillTimer(hWnd, 3);
				ballpt2 = { 0,0 };
				time = 0;
				bMouse = true;
				SetTimer(hWnd, 1, 80 / level, NULL);
			}
			else if (checkball(pt1.x + ballpt2.x, pt1.y - ballpt2.y, rtall.right, rtall.bottom)) {
				//�ʱ�ȭ ����
				KillTimer(hWnd, 3);
				ballpt2 = { 0,0 };
				time = 0;
				bMouse = true;
				SetTimer(hWnd, 1, 80 / level, NULL);
			}
			InvalidateRect(hWnd, NULL, true);
			break;	
		}
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HBRUSH MyBrush, OldBrush;
		HPEN MyPen, OldPen;
		HDC hdc = BeginPaint(hWnd, &ps);
		HFONT hFont, OldFont;
		//��� ���
		for (int i = 0; i <= rtall.right; i = i + 300) {
			DrawBitmap(hdc, rtall.left + i, rtall.top, 300, 500, Backgraund);
		}
		TextOut(hdc, (rtall.right / 2) +200, 50,TEXT("'q'��ư�� ������ â�� ���� �˴ϴ�."),21);
		//������ ���
		DrawBitmap(hdc, (rtall.right/2)-88, 50, 220, 100, MyBitmap1[1]);
		hFont = CreateFont(45, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("����"));
		OldFont = (HFONT)SelectObject(hdc, hFont);
		wsprintf(cscore, TEXT("%d"), score);
		wsprintf(clevel, TEXT("%d"), level);
		DrawText(hdc, cscore, -1, &rtboards, DT_CENTER);
		DrawText(hdc, clevel, -1, &rtboardl, DT_CENTER);
		SelectObject(hdc, OldFont);
		DeleteObject(hFont);

		//������ ���
		DrawBitmap(hdc, rtall.right - 100, objectset, 100, 200, MyBitmap1[0]);

		//�Ŀ������� ���
		DrawBitmap(hdc, 10, 10, 32, 302, MyBitmap2[1]);
		DrawBitmap(hdc, 11, 11, 30, power+300, MyBitmap2[0]);
		TextOut(hdc, 5, 305, TEXT("POWER"), 5);

		//��ǥ�� ���
		MyPen = CreatePen(PS_SOLID, 10, RGB(0,0,0));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		drawarrow(hdc, arrowpointx, arrowpointy, rtall.left, rtall.bottom);

		//��ź ���
		MyBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

		Ellipse(hdc, (pt1.x+ballpt2.x) - BALLSIZE, (pt1.y-ballpt2.y) - BALLSIZE, (pt1.x+ ballpt2.x) + BALLSIZE, (pt1.y-ballpt2.y) + BALLSIZE);
		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);

		EndPaint(hWnd, &ps);
		break; }

	case WM_MOUSEMOVE: {
		if (bMouse)
		{
			arrowpointx = LOWORD(lParam);
			arrowpointy = HIWORD(lParam);
			InvalidateRect(hWnd, &rtarrow, true);
		}
		break;
	}
	case WM_LBUTTONDOWN: {
		if (bMouse)
		{
			SetTimer(hWnd, 2, 50, NULL);
		}
		break;
	}
	case WM_LBUTTONUP: {
		bMouse = false;
		mousept = { arrowpointx, arrowpointy };
		dgree = getdgree(mousept.x, mousept.y, pt1.x, pt1.y);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 1);
		if(power<0)
		SetTimer(hWnd, 3, 100, NULL);
		else {
			ballpt2 = { 0,0 };
			time = 0;
			bMouse = true;
			SetTimer(hWnd, 1, 80 / level, NULL);
		}

		break;
	}
	case WM_CHAR: {
		switch (wParam) {
		case 'q':
			if (MessageBox(hWnd, TEXT("������ �����մϴ�."), TEXT("��������"), MB_OKCANCEL) == IDOK) {
				DestroyWindow(hWnd);
			}
		}
		break;
	}
	case WM_DESTROY: {
					 PostQuitMessage(0);
					 KillTimer(hWnd, 1);
					 KillTimer(hWnd, 2);
					 KillTimer(hWnd, 3);
					 DeleteObject(Backgraund);
					 DeleteObject(MyBitmap1);
					 DeleteObject(MyBitmap2);
					 break;
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam); }
    }
    return 0;
}