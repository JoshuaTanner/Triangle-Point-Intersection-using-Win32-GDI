/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	scene.h
Description	:	scene header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once
#include <Windows.h>
#include <memory>
#include <vector>

#include "BackBuffer.h"

enum Mode{ TRIANGLECREATE, POINTCREATE, FINISHED };

//Point struct
struct Point {
	Point();
	Point(int X, int Y) :x(X), y(Y){}
	int x, y;
};

//line struct
struct Line {
	Line();
	Line(COLORREF color, int StartX, int StartY, int EndX, int EndY)
		:m_color(color), m_iStartX(StartX), m_iStartY(StartY),
		m_iEndX(EndX), m_iEndY(EndY){}
	COLORREF m_color;
	HPEN m_Pen;
	int m_iStartX;
	int m_iEndX;
	int m_iStartY;
	int m_iEndY;
};



//triangle struct
struct Triangle {
	Triangle();
	Triangle(COLORREF color, Point pointA, Point pointB, Point pointC)
		:m_color(color)
	{
		vecPoints.push_back(pointA);
		vecPoints.push_back(pointB);
		vecPoints.push_back(pointC);
	}

	COLORREF m_color;
	HPEN m_Pen;
	std::vector<Point> vecPoints;
};


class CScene
{
private:
	CScene();
	~CScene();
	CScene& operator= (const CScene& _kr);	

protected:
	HINSTANCE m_hAppInstance;
	HWND m_hMainWindow;
	HDC spriteHDC;

	CBackBuffer* m_pBackBuffer;

	static CScene* s_pScene;

public:		
	static CScene& GetInstance();
	bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight);
	void Draw();
	void DestroyInstance();	

	void PointTriangleIntersect();

	Triangle* pTriangle;
	Point* pPoint;
	//Line* lineCutter;
	Mode CurrentMode;
	bool bIntersect;
};

