// EmptyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "EmptyWnd.h"


// CEmptyWnd

IMPLEMENT_DYNAMIC(CEmptyWnd, CScrollWnd)

CEmptyWnd::CEmptyWnd()
{

}

CEmptyWnd::~CEmptyWnd()
{
}

BEGIN_MESSAGE_MAP(CEmptyWnd, CScrollWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CEmptyWnd message handlers



void CEmptyWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	COLORREF bgcolor = pCB->GetThemeColor("PE BG");
	COLORREF textcolor = pCB->GetThemeColor("PE Text");

	CRect cr;
	GetClientRect(&cr);
	dc.FillSolidRect(&cr, bgcolor);

	COLORREF linecolor = Blend(textcolor, bgcolor, 0.2f);
	CPen penl(PS_SOLID, 1, linecolor);
	dc.SelectObject(&penl);

	RECT r;
	GetWindowRect(&r);

	dc.MoveTo(2, cr.bottom - 4);
	dc.LineTo(cr.right - 8, cr.bottom - 4);
	
}
