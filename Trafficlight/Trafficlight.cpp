// Trafficlight.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Trafficlight.h"
#include "Car.h"
#include <queue>
#include <list>
#include <iostream>
#include <string>
using namespace std;


#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
RECT screensize;
INT trafficlightState_V = 1;
INT trafficlightState_H = 3;
int timeToken = 0;
INT CONST Y = 300;
INT CONST X = 500;
INT CONST CAR_LENGTH = Car::getCarLength();
INT CONST CAR_DISTANCE = 10;
std::list <Car> verticalQ;
std::list <Car> horizontalQ;
int CONST SPEED = 10;
int PROBABILITY_V = 100;
int PROBABILITY_H = 100;
bool keyPress = false;
RECT textBox;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MyDLG(HWND, UINT, WPARAM, LPARAM);
VOID drawTrafficlight(HDC hdc, int left, int top, double size, int state);
VOID drawRoad(HDC hdc, INT x, INT y);
INT iterateState(INT state);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRAFFICLIGHT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRAFFICLIGHT));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAFFICLIGHT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRAFFICLIGHT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   SetTimer(hWnd, 0, (1000 / 60), 0);
   textBox.left = 10;
   textBox.top = 10;
   textBox.right = 100;
   textBox.bottom = 200;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_Probability:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MyDLG);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            drawRoad(hdc, X, Y);
            drawTrafficlight(hdc, X - 100, Y - 230, 0.6, trafficlightState_V);
            drawTrafficlight(hdc, X + 100, Y + 40, 0.6, trafficlightState_H);
            for (auto v : verticalQ)
            {
                v.drawCar(hdc);
            }
            for (auto v : horizontalQ) {
                v.drawCar(hdc);
            }
            string message = "pn: " + to_string((double)PROBABILITY_V / (double)100) + "\n" +
                "pw: " + to_string((double)PROBABILITY_H / (double)100);
            wstring stemp = wstring(message.begin(), message.end());
            DrawText(hdc, stemp.c_str(), -1, &textBox, DT_CENTER);

            EndPaint(hWnd, &ps);
        }
        break;

	case WM_LBUTTONDOWN:
	{
	}
	break;
	case WM_RBUTTONDOWN:
	{
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:

			if (PROBABILITY_H >= 10)
			{
				PROBABILITY_H -= 10;
			}
			break;

		case VK_RIGHT:
			PROBABILITY_H += 10;
			break;
		case VK_UP:
			PROBABILITY_V += 10;
			break;
		case VK_DOWN:
			if (PROBABILITY_V >= 10)
			{
				PROBABILITY_V -= 10;
			}
			break;
		}
		InvalidateRect(hWnd, &textBox, true);
		break;
	}
	case WM_TIMER:
	{
		/*
		Iterer den globale tidsvariabelen
		*/
		timeToken++;
		keyPress = false;

		/*
		Fjerner biler som har kjørt ut av skjermen
		*/
		while (!horizontalQ.empty() && horizontalQ.front().getX() > GetSystemMetrics(SM_CXSCREEN) + CAR_LENGTH)
		{
			horizontalQ.pop_front();
		}
		while (!verticalQ.empty() && verticalQ.front().getY() > GetSystemMetrics(SM_CYSCREEN) + CAR_LENGTH)
		{
			verticalQ.pop_front();
		}

		/*
		Endrer på trafikklysene
		*/
		if (timeToken % 360 == 0 || timeToken % 360 == 320)
		{
			trafficlightState_V = iterateState(trafficlightState_V);
			trafficlightState_H = iterateState(trafficlightState_H);
			keyPress = true;
		}
		if (timeToken == 3600) //reset av time sånn at den aldri blir for stor
		{
			timeToken = 0;
		}

		/*
		Spawner nye biler
		*/
		if (rand() % 6000 <= (PROBABILITY_V))
		{
			verticalQ.push_back(*(new Car(true, X, 0)));
		}
		if (rand() % 6000 <= (PROBABILITY_H))
		{
			horizontalQ.push_back(*(new Car(false, 0, Y)));
		}

		/*
		Beveger bilene
		*/
		int previousX = 500000;
		for (auto& c : horizontalQ)
		{
			if (c.getX() < (previousX - (CAR_LENGTH + CAR_DISTANCE)) && (trafficlightState_H == 3 || c.getX() < X - 40 || c.getX() > X - 18))
			{
				c.moveCar(SPEED);
				keyPress = true;
			}
			previousX = c.getX();
		}
		int previousY = 500000;
		for (auto& c : verticalQ)
		{
			if (c.getY() < (previousY - (CAR_LENGTH + CAR_DISTANCE)) && (trafficlightState_V == 3 || c.getY() < Y - 28 || c.getY() > Y - 19))
			{
				c.moveCar(SPEED);
				keyPress = true;
			}
			previousY = c.getY();
		}

		/*
		Oppdaterer bildet
		*/
		if (keyPress)
		{
			InvalidateRect(hWnd, 0, false);
		}
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

INT_PTR CALLBACK MyDLG (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		// Initialize dialog controls with current probability values
		SetDlgItemInt(hDlg, IDC_EDIT_V, PROBABILITY_V, FALSE); // FALSE indicates decimal
		SetDlgItemInt(hDlg, IDC_EDIT_H, PROBABILITY_H, FALSE); // FALSE indicates decimal
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			// Retrieve probability values from dialog controls
			PROBABILITY_V = GetDlgItemInt(hDlg, IDC_EDIT_V, NULL, FALSE);
			PROBABILITY_H = GetDlgItemInt(hDlg, IDC_EDIT_H, NULL, FALSE);
			// Optionally, perform validation or error checking here
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



// Message handler for about box.
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


// Alle Hjelpe funksjonene


VOID drawTrafficlight(HDC hdc, int left, int top, double mult, int state)
{
	HGDIOBJ hOrg = SelectObject(hdc, GetStockObject(DC_BRUSH));

	SetDCBrushColor(hdc, RGB(0, 0, 0));
	//fargelegger øverste sirkel
	if (state == 1 || state == 2)
	{
		SetDCBrushColor(hdc, RGB(204, 50, 50));
	}
	else
	{
		SetDCBrushColor(hdc, RGB(188, 188, 188));
	}
	Ellipse(hdc, left + 10 * mult, top + 10 * mult, left + 90 * mult, top + 90 * mult);

	//fargelegger midterste sirkel
	if (state == 2 || state == 4)
	{
		SetDCBrushColor(hdc, RGB(231, 180, 22));
	}
	else
	{
		SetDCBrushColor(hdc, RGB(188, 188, 188));
	}
	Ellipse(hdc, left + 10 * mult, top + 100 * mult, left + 90 * mult, top + 180 * mult);

	//fargelegger nederste sirkel
	if (state == 3)
	{
		SetDCBrushColor(hdc, RGB(45, 201, 55));
	}
	else
	{
		SetDCBrushColor(hdc, RGB(188, 188, 188));
	}
	Ellipse(hdc, left + 10 * mult, top + 190 * mult, left + 90 * mult, top + 270 * mult);

}

VOID drawRoad(HDC hdc, INT x, INT y)
{
	HGDIOBJ hOrg = SelectObject(hdc, GetStockObject(DC_BRUSH));

	SetDCBrushColor(hdc, RGB(200, 200, 200));

	Rectangle(hdc, 0, y - 20, GetSystemMetrics(SM_CXSCREEN), y + 20);
	Rectangle(hdc, x - 20, 0, x + 20, GetSystemMetrics(SM_CYSCREEN));

	SetDCBrushColor(hdc, RGB(100, 100, 100));
	Rectangle(hdc, x - 20, y - 20, x + 20, y + 20);

}

INT iterateState(INT state)
{
	if (state < 4)
	{
		state++;
	}
	else
	{
		state = 1;
	}

	return state;
}