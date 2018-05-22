#include "stdafx.h"
#include "2DTest.h"
#include "Stat.h"
#include "version.h"



#define Rectangle_(hdc, r) Rectangle(hdc, r.left, r.top, r.right, r.bottom)
#define Ellipse_(hdc, r)   Ellipse(hdc, r.left, r.top, r.right, r.bottom)
#define Line_(hdc, r)   {MoveToEx(hdc, r.left, r.top, NULL); LineTo(hdc, r.right, r.bottom);}
#define Line2_(hdc, x1, y1, x2, y2)   {MoveToEx(hdc, x1, y1, NULL); LineTo(hdc, x2, y2);}
#define arrlen(a) (sizeof(a)/sizeof(a[0]))



#define DRAW_TIMER_PERIOD 500

#define WINDOW_LEFT 10
#define WINDOW_TOP 10


#define SURFACE_MARGIN 4


#define STAT_VALUES_LEFT 200
#define STAT_VALUE_LINE_HEIGHT 16

#define TEXT_SURFACE_WIDTH 340
#define TEXT_SURFACE_HEIGHT (STAT_VALUE_LINE_HEIGHT*5)


#define RESET_BUTTON_LEFT 350
#define RESET_BUTTON_TOP 6
#define RESET_BUTTON_WIDTH 120
#define RESET_BUTTON_HEIGHT 26


#define CONTROLS_LEFT 600

#define CHECKBOX_TOP 4
#define CHECKBOX_WIDTH 180
#define CHECKBOX_HEIGHT 22

#define RADIOBTN_TOP 24

#define RADIOBTN1_WIDTH 80
#define RADIOBTN1_HEIGHT 22

#define RADIOBTN2_WIDTH 80
#define RADIOBTN2_HEIGHT 22

#define IDC_RESET_BUTTON 1
#define IDC_CHECKBOX 10
#define IDC_RADIOBTN_SHAPES 11
#define IDC_RADIOBTN_LINES 12




#define MAX_LOADSTRING 100

HINSTANCE hInst;
HWND hMainWnd;
//TCHAR szTitle[MAX_LOADSTRING] = _T("2DTest");
TCHAR szTitle[MAX_LOADSTRING] = _T(VER_PRODUCTNAME_STR) _T(" v. ") VER_FILE_VERSION_STR_;
TCHAR szWindowClass[MAX_LOADSTRING] = _T("2DTestWindowClass");

HWND g_hwndLastFocus;

RECT ClientRect =
    {0,
     0,
     SURFACE_MARGIN + SHAPES_SURFACE_WIDTH + SURFACE_MARGIN,
     SURFACE_MARGIN + TEXT_SURFACE_HEIGHT + SHAPES_SURFACE_HEIGHT + SURFACE_MARGIN};
RECT TextRect =
    {SURFACE_MARGIN,
     SURFACE_MARGIN,
     SURFACE_MARGIN + SHAPES_SURFACE_WIDTH,
     SURFACE_MARGIN + TEXT_SURFACE_HEIGHT};
RECT ShapesRect =
    {SURFACE_MARGIN,
     SURFACE_MARGIN + TEXT_SURFACE_HEIGHT,
     SURFACE_MARGIN + SHAPES_SURFACE_WIDTH,
     SURFACE_MARGIN + TEXT_SURFACE_HEIGHT + SHAPES_SURFACE_HEIGHT};
RECT InitWindowRect = ClientRect;

bool bAllowWindowResizing = false;
enum GraphicObjects {GraphicObject_Null, GraphicObject_Lines, GraphicObject_Shapes} GraphicObject = GraphicObject_Lines;


RECT Rects[RECTS_COUNT];
RECT Ellipses[ELLIPSES_COUNT];
RECT Lines[LINES_COUNT];


CStat StatTotal;
CStat Stat1;
CStat Stat2;
CStat Stat3;



ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawShapes(HWND);
void DrawDurations(HDC);
void RecalcShapes(const HWND, bool = false);





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;

	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) return FALSE;

    if (!SetTimer(hMainWnd, 0, DRAW_TIMER_PERIOD, NULL)) return FALSE;

    BOOL bRet;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) 
    { 
        if (bRet == -1)
        {
            return 1;
        }
        else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
        {
            DestroyWindow(hMainWnd);
        }
        else if (!IsDialogMessage(hMainWnd, &msg))
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
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= 0;

	return RegisterClassEx(&wcex);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

   if (!AdjustWindowRect(&InitWindowRect, WS_OVERLAPPEDWINDOW, FALSE)) return FALSE;

   hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, WINDOW_LEFT, WINDOW_TOP, InitWindowRect.right, InitWindowRect.bottom, NULL, NULL, hInstance, NULL);
   if (!hMainWnd) return FALSE;

   if (!CreateWindow(_T("BUTTON"), _T("Reset statistics"), WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, RESET_BUTTON_LEFT, RESET_BUTTON_TOP, RESET_BUTTON_WIDTH, RESET_BUTTON_HEIGHT, hMainWnd, (HMENU)IDC_RESET_BUTTON, hInstance, NULL)) return FALSE;
   if (!CreateWindow(_T("BUTTON"), _T("Allow window resizing"), WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_AUTOCHECKBOX, CONTROLS_LEFT, CHECKBOX_TOP, CHECKBOX_WIDTH, CHECKBOX_HEIGHT, hMainWnd, (HMENU)IDC_CHECKBOX, hInstance, NULL)) return FALSE;
   if (!CreateWindow(_T("BUTTON"), _T("Shapes"), WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_AUTORADIOBUTTON, CONTROLS_LEFT,                       RADIOBTN_TOP, RADIOBTN1_WIDTH, RADIOBTN1_HEIGHT, hMainWnd, (HMENU)IDC_RADIOBTN_SHAPES, hInstance, NULL)) return FALSE;
   if (!CreateWindow(_T("BUTTON"), _T("Lines"),  WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_AUTORADIOBUTTON, CONTROLS_LEFT + RADIOBTN1_WIDTH + 8, RADIOBTN_TOP, RADIOBTN2_WIDTH, RADIOBTN2_HEIGHT, hMainWnd, (HMENU)IDC_RADIOBTN_LINES, hInstance, NULL)) return FALSE;
   CheckDlgButton(hMainWnd, IDC_RADIOBTN_LINES, BST_CHECKED);

   ShowWindow(hMainWnd, nCmdShow);
   UpdateWindow(hMainWnd);

   return TRUE;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
    POINT WndSize = {InitWindowRect.right, InitWindowRect.bottom};

	switch (message)
	{
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            bool ClearText = false;
            switch (LOWORD(wParam))
            {
            case IDC_RESET_BUTTON:
                StatTotal.Clear();
                Stat1.Clear();
                Stat2.Clear();
                Stat3.Clear();
                ClearText = true;
                break;

            case IDC_CHECKBOX:
                bAllowWindowResizing = (IsDlgButtonChecked(hMainWnd, IDC_CHECKBOX) == BST_CHECKED);
                if (!bAllowWindowResizing) SetWindowPos(hMainWnd, 0, 0, 0, InitWindowRect.right, InitWindowRect.bottom, SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                RecalcShapes(hWnd);
                break;

            case IDC_RADIOBTN_SHAPES:
            case IDC_RADIOBTN_LINES:
                GraphicObjects NewGraphicObject = GraphicObject_Null;
                if (IsDlgButtonChecked(hMainWnd, IDC_RADIOBTN_SHAPES) == BST_CHECKED)
                    NewGraphicObject = GraphicObject_Shapes;
                else if (IsDlgButtonChecked(hMainWnd, IDC_RADIOBTN_LINES) == BST_CHECKED)
                    NewGraphicObject = GraphicObject_Lines;
                if (GraphicObject != NewGraphicObject)
                {
                    GraphicObject = NewGraphicObject;
                    StatTotal.Clear();
                    Stat1.Clear();
                    Stat2.Clear();
                    Stat3.Clear();
                    ClearText = true;
                    RecalcShapes(hWnd);
                    DrawShapes(hWnd);
                }
                break;
            }

            if (ClearText)
            {
                HDC hdc;
                if (hdc = GetDC(hWnd))
                {
                    RECT r = {SURFACE_MARGIN, SURFACE_MARGIN, TEXT_SURFACE_WIDTH, TEXT_SURFACE_HEIGHT};
                    FillRect(hdc, &r, (HBRUSH) (COLOR_BTNFACE+1));
                    DrawDurations(hdc);
                    ReleaseDC(hWnd, hdc);
                };
            }
        }
        break;

    case WM_ACTIVATE:
        if (wParam == WA_INACTIVE) g_hwndLastFocus = GetFocus();
        break;

    case WM_SETFOCUS:
        if (g_hwndLastFocus) SetFocus(g_hwndLastFocus);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
        break;

    case WM_TIMER:
        DrawShapes(hWnd);
        break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
        DrawShapes(hWnd);
		EndPaint(hWnd, &ps);
		break;

    case WM_SIZE:
        if (bAllowWindowResizing) RecalcShapes(hWnd);
        break;

    case WM_CREATE:
        RecalcShapes(hWnd, true);
        break;

    case WM_GETMINMAXINFO:
        if (!bAllowWindowResizing)
        {
            ((LPMINMAXINFO)lParam)->ptMaxSize = WndSize;
            ((LPMINMAXINFO)lParam)->ptMinTrackSize = WndSize;
            ((LPMINMAXINFO)lParam)->ptMaxTrackSize = WndSize;
        }
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

    SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));

    switch (GraphicObject)
    {
    case GraphicObject_Lines:

        _tcscpy_s(Str1, _T("Line drawing time:"));
        TextOut(hdc, SURFACE_MARGIN, SURFACE_MARGIN, Str1, _tcslen(Str1));
        _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), Stat3.Avg());
        _tcscat_s(Str1, _T(" msec    "));
        TextOut(hdc, STAT_VALUES_LEFT, SURFACE_MARGIN, Str1, _tcslen(Str1));
        break;

    case GraphicObject_Shapes:

        _tcscpy_s(Str1, _T("Total drawing time:"));
        TextOut(hdc, SURFACE_MARGIN, SURFACE_MARGIN, Str1, _tcslen(Str1));
        _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), StatTotal.Avg());
        _tcscat_s(Str1, _T(" msec    "));
        TextOut(hdc, STAT_VALUES_LEFT, SURFACE_MARGIN, Str1, _tcslen(Str1));

        _tcscpy_s(Str1, _T("Rectangles drawing time: "));
        TextOut(hdc, SURFACE_MARGIN, SURFACE_MARGIN*2+STAT_VALUE_LINE_HEIGHT, Str1, _tcslen(Str1));
        _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), Stat1.Avg());
        _tcscat_s(Str1, _T(" msec    "));
        TextOut(hdc, STAT_VALUES_LEFT, SURFACE_MARGIN*2+STAT_VALUE_LINE_HEIGHT, Str1, _tcslen(Str1));

        _tcscpy_s(Str1, _T("Ellipses drawing time:"));
        TextOut(hdc, SURFACE_MARGIN, SURFACE_MARGIN*2+2*STAT_VALUE_LINE_HEIGHT, Str1, _tcslen(Str1));
        _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), Stat2.Avg());
        _tcscat_s(Str1, _T(" msec    "));
        TextOut(hdc, STAT_VALUES_LEFT, SURFACE_MARGIN*2+2*STAT_VALUE_LINE_HEIGHT, Str1, _tcslen(Str1));

        _tcscpy_s(Str1, _T("Lines drawing time:"));
        TextOut(hdc, SURFACE_MARGIN, SURFACE_MARGIN*2+3*STAT_VALUE_LINE_HEIGHT, Str1, _tcslen(Str1));
        _sntprintf_s(Str1, _countof(Str1), _T("%.6f"), Stat3.Avg());
        _tcscat_s(Str1, _T(" msec    "));
        TextOut(hdc, STAT_VALUES_LEFT, SURFACE_MARGIN*2+3*STAT_VALUE_LINE_HEIGHT, Str1, _tcslen(Str1));
        break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawShapes(HWND hWnd)
{
    HDC hdc;
    int x, y;
    COLORREF PenColor[] = {RGB(0,0,0), RGB(255,0,0), RGB(0,0,255)};
    int i;
    LARGE_INTEGER Frequency;
    LARGE_INTEGER StartTimeTotal, EndTimeTotal;
    LARGE_INTEGER StartTime1, EndTime1;
    LARGE_INTEGER StartTime2, EndTime2;
    LARGE_INTEGER StartTime3, EndTime3;



    if (!(hdc = GetDC(hWnd))) return;

    switch (GraphicObject)
    {
    case GraphicObject_Null:
        FillRect(hdc, &ShapesRect, (HBRUSH) (COLOR_WINDOW+1));
        break;

    case GraphicObject_Lines:

        SelectObject(hdc, GetStockObject(DC_PEN));
        FillRect(hdc, &ShapesRect, (HBRUSH) (COLOR_WINDOW+1));
        
        QueryPerformanceFrequency(&Frequency);

        QueryPerformanceCounter(&StartTime3);
        for (i = 0; i < arrlen(PenColor); i++)
        {
            SetDCPenColor(hdc, PenColor[i]);
            for (y = ShapesRect.top; y < ShapesRect.bottom; y+=4) Line2_(hdc, ShapesRect.left, y, ShapesRect.right, y);
            for (x = ShapesRect.left; x < ShapesRect.right; x+=4) Line2_(hdc, x, ShapesRect.top, x, ShapesRect.bottom);
            //for (x = ShapesRect.left; x < ShapesRect.right-100; x+=20) Line2_(hdc, x, ShapesRect.top, x+100, ShapesRect.bottom);
            //for (x = ShapesRect.left; x < ShapesRect.right-100; x+=20) Line2_(hdc, x+100, ShapesRect.top, x, ShapesRect.bottom);
        }
        QueryPerformanceCounter(&EndTime3);

        Stat3.Add((EndTime3.QuadPart - StartTime3.QuadPart) * 1000.0 / Frequency.QuadPart);

        DrawDurations(hdc);

        break;

    case GraphicObject_Shapes:

        FillRect(hdc, &ShapesRect, (HBRUSH) (COLOR_WINDOW+1));

        QueryPerformanceFrequency(&Frequency);
        
        QueryPerformanceCounter(&StartTimeTotal);
        
        QueryPerformanceCounter(&StartTime1);
        SetDCPenColor(hdc, PenColor[0]);
        SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
        for (int i = 0; i < RECTS_COUNT; i++) Rectangle_(hdc, Rects[i]) ;
        QueryPerformanceCounter(&EndTime1);

        QueryPerformanceCounter(&StartTime2);
        SetDCPenColor(hdc, PenColor[1]);
        SelectObject(hdc, GetStockObject(DKGRAY_BRUSH));
        for (int i = 0; i < ELLIPSES_COUNT; i++) Ellipse_(hdc, Ellipses[i]) ;
        QueryPerformanceCounter(&EndTime2);

        QueryPerformanceCounter(&StartTime3);
        SetDCPenColor(hdc, PenColor[2]);
        for (int i = 0; i < LINES_COUNT; i++) Line_(hdc, Lines[i]) ;
        QueryPerformanceCounter(&EndTime3);

        QueryPerformanceCounter(&EndTimeTotal);


        StatTotal.Add((EndTimeTotal.QuadPart - StartTimeTotal.QuadPart) * 1000.0 / Frequency.QuadPart);
        Stat1.Add((EndTime1.QuadPart - StartTime1.QuadPart) * 1000.0 / Frequency.QuadPart);
        Stat2.Add((EndTime2.QuadPart - StartTime2.QuadPart) * 1000.0 / Frequency.QuadPart);
        Stat3.Add((EndTime3.QuadPart - StartTime3.QuadPart) * 1000.0 / Frequency.QuadPart);

        DrawDurations(hdc);

        break;
    }

    ReleaseDC(hWnd, hdc);

}

    


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RecalcShapes(const HWND hWnd, bool Forced)
{
    RECT cr;
    GetClientRect(hWnd, &cr);
    bool Changed = (ClientRect.left != cr.left) || (ClientRect.top != cr.top) || (ClientRect.right != cr.right) || (ClientRect.bottom != cr.bottom);

    if (Changed || Forced)
    {
        ClientRect = cr;
        TextRect.right = ClientRect.right - SURFACE_MARGIN;
        ShapesRect.right = ClientRect.right - SURFACE_MARGIN;
        ShapesRect.bottom = ClientRect.bottom - SURFACE_MARGIN;

        
        int w = ShapesRect.right - ShapesRect.left;
        int h = ShapesRect.bottom - ShapesRect.top;
        int i;
        for (i = 0; i < RECTS_COUNT; i++)
        {
            Rects[i].left   = ShapesRect.left + SrcRects[i].left * w / SHAPES_SURFACE_WIDTH;
            Rects[i].right  = ShapesRect.left + SrcRects[i].right * w / SHAPES_SURFACE_WIDTH;
            Rects[i].top    = ShapesRect.top + SrcRects[i].top * h / SHAPES_SURFACE_HEIGHT;
            Rects[i].bottom = ShapesRect.top + SrcRects[i].bottom * h / SHAPES_SURFACE_HEIGHT;
        }
        for (i = 0; i < ELLIPSES_COUNT; i++)
        {
            Ellipses[i].left   = ShapesRect.left + SrcEllipses[i].left * w / SHAPES_SURFACE_WIDTH;
            Ellipses[i].right  = ShapesRect.left + SrcEllipses[i].right * w / SHAPES_SURFACE_WIDTH;
            Ellipses[i].top    = ShapesRect.top + SrcEllipses[i].top * h / SHAPES_SURFACE_HEIGHT;
            Ellipses[i].bottom = ShapesRect.top + SrcEllipses[i].bottom * h / SHAPES_SURFACE_HEIGHT;
        }
        for (i = 0; i < LINES_COUNT; i++)
        {
            Lines[i].left   = ShapesRect.left + SrcLines[i].left * w / SHAPES_SURFACE_WIDTH;
            Lines[i].right  = ShapesRect.left + SrcLines[i].right * w / SHAPES_SURFACE_WIDTH;
            Lines[i].top    = ShapesRect.top + SrcLines[i].top * h / SHAPES_SURFACE_HEIGHT;
            Lines[i].bottom = ShapesRect.top + SrcLines[i].bottom * h / SHAPES_SURFACE_HEIGHT;
        }

    }
}


