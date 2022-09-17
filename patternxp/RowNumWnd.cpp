// RowNumWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RowNumWnd.h"
#include "EditorWnd.h"
#include "PatEd.h"

// CRowNumWnd

IMPLEMENT_DYNAMIC(CRowNumWnd, CScrollWnd)

CRowNumWnd::CRowNumWnd()
{

}

CRowNumWnd::~CRowNumWnd()
{
}


BEGIN_MESSAGE_MAP(CRowNumWnd, CScrollWnd)

END_MESSAGE_MAP()


void CRowNumWnd::OnDraw(CDC *pDC)
{
	CRect clr;
	GetClientRect(&clr);
	CRect cr = GetCanvasRect();

	CRect ur;
	ur.UnionRect(&cr, &clr);

	CRect clipbox;
	pDC->GetClipBox(&clipbox);
	
	COLORREF bgcolor = pew->pCB->GetThemeColor("PE BG");
	COLORREF textcolor = pew->pCB->GetThemeColor("PE Text");

	pDC->FillSolidRect(&ur, bgcolor);

	if (pew->pPattern == NULL)
		return;

	CObject *pOldFont = pDC->SelectObject(&pew->font);

	pDC->SetTextColor(textcolor);

	int const firstrow = max(0, clipbox.top / pew->fontSize.cy);
	int const lastrow = min(pew->pPattern->GetRowCount(), clipbox.bottom / pew->fontSize.cy + 1);

	for (int y = firstrow; y < lastrow; y++)
	{
		char buf[8];
		sprintf(buf, "%5d  ", y);
		pDC->TextOut(-3, y * pew->fontSize.cy, buf, 5);
	}

	COLORREF linecolor = Blend(textcolor, bgcolor, 0.2f);
	CPen penl(PS_SOLID, 1, linecolor);
	CObject* pOldPen = pDC->SelectObject(&penl);

	RECT r;
	GetWindowRect(&r);

	pDC->MoveTo(clr.right - 4, 2);
	pDC->LineTo(clr.right - 4, r.bottom - r.top - 6);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

	//--
	CRect rp = pew->pe.GetCursorRect();
	COLORREF curcol = pew->pe.curcol;
		
	Gdiplus::Pen pen(Gdiplus::Color(150, 0, 0, 0), 1);
	Gdiplus::Graphics g(pDC->GetSafeHdc());

	Gdiplus::Color color(50, GetRValue(curcol), GetGValue(curcol), GetBValue(curcol));
		
	Gdiplus::Rect rectangle(1, rp.top, clr.right - 6, rp.bottom - rp.top);
	Gdiplus::SolidBrush solidBrush(color);
		
	g.FillRectangle(&solidBrush, rectangle);
		
	rectangle.Width -= 1;
	rectangle.Height -= 1;
	g.DrawRectangle(&pen, rectangle);
		
}



// CRowNumWnd message handlers


