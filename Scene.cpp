#include "Scene.h"

#define VALIDATE(a) if (!a) return (false)
CScene* CScene::s_pScene = 0;

CScene::CScene()
	:m_hAppInstance(0)
	, m_pBackBuffer(0)	
{

}

CScene::~CScene()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;	

	//for (unsigned int i = 0; i < vecLines.size(); i++)
	//{
	//	delete vecLines[i];
	//	vecLines[i] = 0;
	//}

}

float DotProduct(Point& rPointA, Point& rPointB)
{
	float _fDotProduct = rPointA.x*rPointB.x + rPointA.y*rPointB.y;
	return(_fDotProduct);
}


void CScene::PointTriangleIntersect()
{
	float fTotalAngle = 0.0f;
	float fTheta;

	//Triangle point - point
	Point vecPointToCorner0(pTriangle->vecPoints[0].x - pPoint->x, pTriangle->vecPoints[0].y - pPoint->y);
	Point vecPointToCorner1(pTriangle->vecPoints[1].x - pPoint->x, pTriangle->vecPoints[1].y - pPoint->y);
	Point vecPointToCorner2(pTriangle->vecPoints[2].x - pPoint->x, pTriangle->vecPoints[2].y - pPoint->y);

	float fDot = DotProduct(vecPointToCorner0, vecPointToCorner1);
	float m1 = sqrt(vecPointToCorner0.x*vecPointToCorner0.x + vecPointToCorner0.y*vecPointToCorner0.y);
	float m2 = sqrt(vecPointToCorner1.x*vecPointToCorner1.x + vecPointToCorner1.y*vecPointToCorner1.y);

	//Solve for theta
	fTheta = acos(fDot / (m1*m2));
	fTotalAngle += fTheta;

	//Triangle point - point
	fDot = DotProduct(vecPointToCorner1, vecPointToCorner2);
	m1 = sqrt(vecPointToCorner1.x*vecPointToCorner1.x + vecPointToCorner1.y*vecPointToCorner1.y);
	m2 = sqrt(vecPointToCorner2.x*vecPointToCorner2.x + vecPointToCorner2.y*vecPointToCorner2.y);

	fTheta = acos(fDot / (m1*m2));
	fTotalAngle += fTheta;

	//Triangle point - point
	fDot = DotProduct(vecPointToCorner2, vecPointToCorner0);
	m1 = sqrt(vecPointToCorner2.x*vecPointToCorner2.x + vecPointToCorner2.y*vecPointToCorner2.y);
	m2 = sqrt(vecPointToCorner0.x*vecPointToCorner0.x + vecPointToCorner0.y*vecPointToCorner0.y);

	fTheta = acos(fDot / (m1*m2));
	fTotalAngle += fTheta;

	//Convert to degrees
	fTotalAngle *= (180.0f / 3.14150f);

	//Check for intersection
	if (fTotalAngle > 359.9f && fTotalAngle < 360.1f)
	{
		bIntersect = true;
	}
	else
	{
		bIntersect = false;
	}
	
}

void CScene::DestroyInstance()
{
	delete s_pScene;
	s_pScene = 0;
}

CScene& CScene::GetInstance()
{
	if (s_pScene == 0)
	{
		s_pScene = new CScene();
	}

	return (*s_pScene);
}


bool CScene::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	m_hAppInstance = _hInstance;
	m_hMainWindow = _hWnd;

	m_pBackBuffer = new CBackBuffer();
	VALIDATE(m_pBackBuffer->Initialise(_hWnd, _iWidth, _iHeight));
	
	Point A(0, 0);
	pTriangle = new Triangle(RGB(0.0f, 0.0f, 0.0f),A,A,A);
	pTriangle->vecPoints.clear();
	
	HINSTANCE hInstance = m_hAppInstance;
	if (!spriteHDC)
		spriteHDC = CreateCompatibleDC(NULL);	

	return true;
}


void CScene::Draw()
{
	m_pBackBuffer->Clear();
	int _iWidth, _iHeight, _iX, _iY;
	if (CurrentMode == POINTCREATE || CurrentMode == FINISHED){
		
			TRIVERTEX vertex[3];
			vertex[0].x = pTriangle->vecPoints[0].x;
			vertex[0].y = pTriangle->vecPoints[0].y;
			vertex[0].Red = 0xff00;
			vertex[0].Green = 0x8000;
			vertex[0].Blue = 0x0000;
			vertex[0].Alpha = 0x0000;

			vertex[1].x = pTriangle->vecPoints[1].x;
			vertex[1].y = pTriangle->vecPoints[1].y;
			vertex[1].Red = 0x9000;
			vertex[1].Green = 0x0000;
			vertex[1].Blue = 0x9000;
			vertex[1].Alpha = 0x0000;

			vertex[2].x = pTriangle->vecPoints[2].x;
			vertex[2].y = pTriangle->vecPoints[2].y;
			vertex[2].Red = 0x9000;
			vertex[2].Green = 0x0000;
			vertex[2].Blue = 0x9000;
			vertex[2].Alpha = 0x0000;

			GRADIENT_TRIANGLE gTriangle;
			gTriangle.Vertex1 = 0;
			gTriangle.Vertex2 = 1;
			gTriangle.Vertex3 = 2;

			GradientFill(m_pBackBuffer->GetBFDC(), vertex, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
		
	}
	if (pPoint != NULL && CurrentMode == FINISHED){
		HPEN pointPen = CreatePen(PS_SOLID, 10, RGB(10.0f,0.0f,10.0f));
		HPEN old_Pen = static_cast<HPEN>(SelectObject(m_pBackBuffer->GetBFDC(), pointPen));
		MoveToEx(m_pBackBuffer->GetBFDC(), ((pPoint->x)), ((pPoint->y)), NULL);
		LineTo(m_pBackBuffer->GetBFDC(), ((pPoint->x)), ((pPoint->y)));
		SelectObject(m_pBackBuffer->GetBFDC(), old_Pen);
		DeleteObject(pointPen);
	}

	if (CurrentMode == FINISHED)
	{
		if (bIntersect == true)
		{
			TextOut(m_pBackBuffer->GetBFDC(), 10, 65, L"Point is intersecting triangle", 30);
		}
		else
		{
			TextOut(m_pBackBuffer->GetBFDC(), 10, 65, L"Point is not intersecting triangle", 35);
		}

	}

	TextOut(m_pBackBuffer->GetBFDC(), 10, 5, L"Click thrice to make triangle", 29);
	TextOut(m_pBackBuffer->GetBFDC(), 10, 25, L"Click again to make point", 25);
	TextOut(m_pBackBuffer->GetBFDC(), 10, 45, L"Right click to reset", 20);

	m_pBackBuffer->Present();
}