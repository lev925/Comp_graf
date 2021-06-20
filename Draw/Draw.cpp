// simple.cpp: пример простейшего Windows-приложения.
//

//#undef UNICODE
//#undef _UNICODE

#include "framework.h"
#include "Draw.h"
#include <fstream>
#include <ctime>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>

#define M_PI  3.14159265358979323846
#define MAX_LOADSTRING 100
#define _CRT_SECURE_NO_WARNINGS

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Глобальная структура для параметров программы.
struct Parameters
{
	int x1, x2, y1, y2, mode1, mode2, n;
	std::vector<std::vector<int>> vec;
} parameters;



//
//   ФУНКЦИЯ: ReadParameters()
//
//   НАЗНАЧЕНИЕ: присваивает глобальным переменным значения параметров программы.
//
//   КОММЕНТАРИИ:
//
//        Тело функции реализовано простейшим способом. Обычно присваиваемые значения 
//		  читаются из файла настроек или командной строки.
//
void ReadParameters()
{
	std::ifstream fin;
	fin.open("parameters.dat");
	fin >> parameters.mode1;
	fin >> parameters.mode2;

	if (parameters.mode1 == 1)
	{
		switch (parameters.mode2)
		{
		case 1:
			parameters.x1 = 300;
			parameters.y1 = 300;
			parameters.x2 = 400;
			parameters.y2 = 300;
		case 2:
			fin >> parameters.n;
			break;
		case 3:
			fin >> parameters.x1;
			fin >> parameters.y1;
			fin >> parameters.x2;
			fin >> parameters.y2;
			break;
		}
	}

	fin.close();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Читать значения параметров программы из файла или командной строки 
	ReadParameters();


	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
//  КОММЕНТАРИИ:
//
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный код
//    был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void drawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;

	int error = deltaX - deltaY;

	SetPixel(hdc, x2, y2, RGB(50, 250, 0));
	while (x1 != x2 || y1 != y2)
	{
		SetPixel(hdc, x1, y1, RGB(250, 50, 0));
		const int error2 = error * 2;

		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}

}

void swap(std::vector<int>& a, std::vector<int>& b)
{
	std::vector<int> c;
	c = a;
	a = b;
	b = c;
}

void fillPolygon(HDC hdc, std::vector<std::vector<int>> vec)
{
	std::map<int, std::vector<int>> a;
	std::map<int, std::vector<int>>::iterator it;
	int i, j;
	int x1, y1, x2, y2;

	for (i = 0; i < vec.size() - 1; i++)
	{
		if (vec[i][1] < vec[i + 1][1])
		{ 
			x1 = vec[i][0];
			x2 = vec[i + 1][0];
			y1 = vec[i][1];
			y2 = vec[i + 1][1];
		}
		else 
		{ 
			x1 = vec[i + 1][0];
			x2 = vec[i][0];
			y1 = vec[i + 1][1];
			y2 = vec[i][1];
		}
		for (j = y1; j < y2; j++)
			a[j].push_back((j - y1) * (x2 - x1) / (y2 - y1) + x1);
	}
	if (vec[0][1] < vec[vec.size() - 1][1]) 
	{ 
		x1 = vec[0][0];
		x2 = vec[vec.size() - 1][0];
		y1 = vec[0][1];
		y2 = vec[vec.size() - 1][1];
	}
	else 
	{ 
		x1 = vec[vec.size() - 1][0];
		x2 = vec[0][0];
		y1 = vec[vec.size() - 1][1];
		y2 = vec[0][1]; 
	}
	for (j = y1; j < y2; j++)
		a[j].push_back((j - y1) * (x2 - x1) / (y2 - y1) + x1);

	for (it = a.begin(); it != a.end(); it++)
	{
		std::sort(it->second.begin(), it->second.end());
		drawLine(hdc, it->second[0], it->first, it->second[1], it->first);
	}
}

void myProg(HDC hdc)
{
	srand(time(0));

	if (parameters.mode1 == 1)
		switch (parameters.mode2)
		{
		case 1:
			for (int i(0); i < parameters.n; i++)
			{
				drawLine(hdc, parameters.x1, parameters.y1, parameters.x1 + (parameters.x2 - parameters.x1) * std::cos(2 * i * M_PI / parameters.n) + (parameters.y2 - parameters.y1) * std::sin(2 * i * M_PI / parameters.n), parameters.y1 - (parameters.x2 - parameters.x1) * std::sin(2 * i * M_PI / parameters.n) + (parameters.y2 - parameters.y1) * std::cos(2 * i * M_PI / parameters.n));
			}
			break;
		case 2:
			for (int i(0); i < parameters.n; i++)
				drawLine(hdc, rand() % 500, rand() % 500, rand() % 500, rand() % 500);
			break;
		case 3:
			drawLine(hdc, parameters.x1, parameters.y1, parameters.x2, parameters.y2);
		}
	else
	{
		if (parameters.mode2 == 1)
		{
			std::vector<std::vector<int>> vec = { {20,40},{80,20},{140,20},{200,100},{180,180},{120,200},{20,140} };
			fillPolygon(hdc, vec);

			vec = { {420,240},{400,300},{480,260},{440,320},{480,360},{460,400},{360,400},{360,360},{380,320},{320,320} };
			fillPolygon(hdc, vec);
		}
		else
		{
			int i, n, x, y;
			std::ifstream fin;
			fin.open("FillPoly.txt");

			while (!fin.eof())
			{
				std::vector<std::vector<int>> vec;
				fin >> n;
				for (int i(0); i < n; i++)
				{
					fin >> x >> y;
					vec.push_back({ x,y });
				}
				fillPolygon(hdc, vec);
				vec.clear();
			}
		}
	}
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Нарисовать горизонтальную прямую с параметрами parameters  
		myProg(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
