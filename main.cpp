//Library Includes
#include <windows.h>
#include <windowsx.h>

//Local Includes
#include "Scene.h"

#define WINDOW_CLASS_NAME L"Triangle Intersection"



CScene* g_pScene;

int PointX, PointY;


LRESULT CALLBACK
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	int XCoord, YCoord;

	switch (_uiMsg)
	{
	case WM_CREATE:
	{
		g_pScene->CurrentMode = TRIANGLECREATE;	
		break;
		return(0);
	}
	case WM_COMMAND:
	{
		switch (LOWORD(_wParam))
		{
	

		}
		break;
		return(0);
	}

	case WM_LBUTTONDOWN:
	{
		XCoord = (static_cast<int>(LOWORD(_lParam)));
		YCoord = (static_cast<int>(HIWORD(_lParam)));

		if (g_pScene->CurrentMode == TRIANGLECREATE)
		{
			if (g_pScene->pTriangle->vecPoints.size() < 3)
			{
				Point newPoint(XCoord, YCoord);
				g_pScene->pTriangle->vecPoints.push_back(newPoint);
			}
			if (g_pScene->pTriangle->vecPoints.size() == 3){
				g_pScene->CurrentMode = POINTCREATE;
			}
		}	
		else if (g_pScene->CurrentMode == POINTCREATE)
		{
				PointX = XCoord;
				PointY = YCoord;
				g_pScene->pPoint = new Point(PointX, PointY);
				g_pScene->PointTriangleIntersect();
				g_pScene->CurrentMode = FINISHED;	
		
		}

		break;
		return(0);
	}	
	case WM_RBUTTONDOWN:
	{
		g_pScene->CurrentMode = TRIANGLECREATE;
		g_pScene->pTriangle->vecPoints.clear();
		break;
		return(0);
	}

	case WM_DESTROY:
	{		
		PostQuitMessage(0);
		return TRUE;
		return(0);
	}
		break;

	default:break;
	}

	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}



HWND
CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, const wchar_t* _pcTitle)
{
	HBRUSH WhiteScreen = CreateSolidBrush(RGB(0, 0, 0));
	WNDCLASSEX winclass;
	winclass.hInstance = _hInstance;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_HAND);
	winclass.hbrBackground = static_cast<HBRUSH> (GetStockObject(NULL_BRUSH));
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);



	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	HWND hwnd;
	hwnd = CreateWindowEx(NULL,
		WINDOW_CLASS_NAME,
		_pcTitle,
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		_iWidth, _iHeight,
		NULL,
		NULL,
		_hInstance,
		NULL);

	if (!hwnd)
	{
		// Failed to create.
		return (0);
	}

	return (hwnd);
}

int WINAPI
WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline, int _iCmdshow)
{
	MSG msg;
	RECT _rect;
	ZeroMemory(&msg, sizeof(MSG));
	const int kiWidth = 1200;
	const int kiHeight = 900;

	g_pScene = &CScene::GetInstance();	

	HWND hwnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, L"Triangle Intersection");
	g_pScene->Initialise(_hInstance, hwnd, kiWidth, kiHeight);

	GetClientRect(hwnd, &_rect);
	

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_pScene->Draw();
		}
	}


	return (static_cast<int>(msg.wParam));
}
