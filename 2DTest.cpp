#include "stdafx.h"
#include "2DTest.h"


#define Rectangle_(hdc, r) Rectangle(hdc, r.left, r.top, r.right, r.bottom)
#define Ellipse_(hdc, r)   Ellipse(hdc, r.left, r.top, r.right, r.bottom)
#define Line_(hdc, r)   {MoveToEx(hdc, r.left, r.top, NULL); LineTo(hdc, r.right, r.bottom);}




#define WINDOW_LEFT 10
#define WINDOW_TOP 10
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700




#define MAX_LOADSTRING 100

HINSTANCE hInst;
HWND hMainWnd;
TCHAR szTitle[MAX_LOADSTRING] = _T("2DTest");
TCHAR szWindowClass[MAX_LOADSTRING] = _T("2DTestWindowClass");

std::vector<double> ElapsedTimeTotalArray;
std::vector<double> ElapsedTime1Array;
std::vector<double> ElapsedTime2Array;
std::vector<double> ElapsedTime3Array;
double MedianTotal = 0;
double Median1 = 0;
double Median2 = 0;
double Median3 = 0;


ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawShapes(HWND hWnd);
void DrawDurations(HDC hdc);




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable = 0;

	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) return FALSE;

    if (!SetTimer(hMainWnd, 0, 1000, NULL)) return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_OWNDC | CS_SAVEBITS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= 0;

	return RegisterClassEx(&wcex);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, WINDOW_LEFT, WINDOW_TOP, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
   if (!hMainWnd) return FALSE;

   ShowWindow(hMainWnd, nCmdShow);
   UpdateWindow(hMainWnd);

   return TRUE;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
    POINT WndSize = {WINDOW_WIDTH, WINDOW_HEIGHT};

	switch (message)
	{
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
        break;

    case WM_TIMER:
        DrawShapes(hWnd);
        break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
        DrawDurations(hdc);
		EndPaint(hWnd, &ps);
		break;

    case WM_GETMINMAXINFO:
        ((LPMINMAXINFO)lParam)->ptMaxSize = WndSize;
        ((LPMINMAXINFO)lParam)->ptMinTrackSize = WndSize;
        ((LPMINMAXINFO)lParam)->ptMaxTrackSize = WndSize;
        break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawDurations(HDC hdc)
{
    TCHAR Str1[100];

    _tcscpy_s(Str1, _T("Общее время отрисовки:"));
    TextOut(hdc, 5, 4, Str1, _tcslen(Str1));
    _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), MedianTotal);
    _tcscat_s(Str1, _T(" мсек"));
    TextOut(hdc, 250, 4, Str1, _tcslen(Str1));

    _tcscpy_s(Str1, _T("Время отрисовки прямоугольников: "));
    TextOut(hdc, 5, 8+15, Str1, _tcslen(Str1));
    _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), Median1);
    _tcscat_s(Str1, _T(" мсек"));
    TextOut(hdc, 250, 8+15, Str1, _tcslen(Str1));

    _tcscpy_s(Str1, _T("Время отрисовки эллипсов:"));
    TextOut(hdc, 5, 8+2*15, Str1, _tcslen(Str1));
    _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), Median2);
    _tcscat_s(Str1, _T(" мсек"));
    TextOut(hdc, 250, 8+2*15, Str1, _tcslen(Str1));

    _tcscpy_s(Str1, _T("Время отрисовки линий:"));
    TextOut(hdc, 5, 8+3*15, Str1, _tcslen(Str1));
    _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), Median3);
    _tcscat_s(Str1, _T(" мсек"));
    TextOut(hdc, 250, 8+3*15, Str1, _tcslen(Str1));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawShapes(HWND hWnd)
{
    HDC hdc;
    double ElapsedTimeTotal = 0;
    double ElapsedTime1 = 0;
    double ElapsedTime2 = 0;
    double ElapsedTime3 = 0;
    LARGE_INTEGER Frequency;
    LARGE_INTEGER StartTimeTotal, EndTimeTotal;
    LARGE_INTEGER StartTime1, EndTime1;
    LARGE_INTEGER StartTime2, EndTime2;
    LARGE_INTEGER StartTime3, EndTime3;


    if (!(hdc = GetDC(hWnd))) return;

    QueryPerformanceFrequency(&Frequency);
    
    SelectObject(hdc, GetStockObject(BLACK_PEN));


    QueryPerformanceCounter(&StartTimeTotal);
    
    SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
    QueryPerformanceCounter(&StartTime1);
    for (int i = 0; i < RECTS_COUNT; i++) Rectangle_(hdc, Rects[i]) ;
    QueryPerformanceCounter(&EndTime1);

    SelectObject(hdc, GetStockObject(DKGRAY_BRUSH));
    QueryPerformanceCounter(&StartTime2);
    for (int i = 0; i < ELLIPSES_COUNT; i++) Ellipse_(hdc, Ellipses[i]) ;
    QueryPerformanceCounter(&EndTime2);

    QueryPerformanceCounter(&StartTime3);
    for (int i = 0; i < LINES_COUNT; i++) Line_(hdc, Lines[i]) ;
    QueryPerformanceCounter(&EndTime3);

    QueryPerformanceCounter(&EndTimeTotal);


    ElapsedTimeTotal = (EndTimeTotal.QuadPart - StartTimeTotal.QuadPart) * 1000.0 / Frequency.QuadPart;
    ElapsedTime1 = (EndTime1.QuadPart - StartTime1.QuadPart) * 1000.0 / Frequency.QuadPart;
    ElapsedTime2 = (EndTime2.QuadPart - StartTime2.QuadPart) * 1000.0 / Frequency.QuadPart;
    ElapsedTime3 = (EndTime3.QuadPart - StartTime3.QuadPart) * 1000.0 / Frequency.QuadPart;


    ElapsedTimeTotalArray.push_back(ElapsedTimeTotal);
    std::nth_element(ElapsedTimeTotalArray.begin(), ElapsedTimeTotalArray.begin() + ElapsedTimeTotalArray.size()/2, ElapsedTimeTotalArray.end());
    MedianTotal = ElapsedTimeTotalArray[ElapsedTimeTotalArray.size() / 2];

    ElapsedTime1Array.push_back(ElapsedTime1);
    std::nth_element(ElapsedTime1Array.begin(), ElapsedTime1Array.begin() + ElapsedTime1Array.size()/2, ElapsedTime1Array.end());
    Median1 = ElapsedTime1Array[ElapsedTime1Array.size() / 2];

    ElapsedTime2Array.push_back(ElapsedTime2);
    std::nth_element(ElapsedTime2Array.begin(), ElapsedTime2Array.begin() + ElapsedTime2Array.size()/2, ElapsedTime2Array.end());
    Median2 = ElapsedTime2Array[ElapsedTime2Array.size() / 2];

    ElapsedTime3Array.push_back(ElapsedTime3);
    std::nth_element(ElapsedTime3Array.begin(), ElapsedTime3Array.begin() + ElapsedTime3Array.size()/2, ElapsedTime3Array.end());
    Median3 = ElapsedTime3Array[ElapsedTime3Array.size() / 2];

    DrawDurations(hdc);

    ReleaseDC(hWnd, hdc);
}

    
