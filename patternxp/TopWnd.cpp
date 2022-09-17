// TopWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TopWnd.h"
#include "EditorWnd.h"
#include <winuser.h>

// CTopWnd

IMPLEMENT_DYNAMIC(CTopWnd, CScrollWnd)

#define INITIALX_96DPI_TOP_WND_FONT 90 
#define INITIALX_96DPI_TOP_WND_FONT2 20

CTopWnd::CTopWnd()
{
	lineHeight = 20;
	toolTipEnalbed = true;
}

CTopWnd::~CTopWnd()
{	
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_CANCEL | TME_LEAVE;
	tme.dwHoverTime = 0;
	TrackMouseEvent(&tme);

	::SendMessage(hwndTT, WM_CLOSE, 0, 0);
}


BEGIN_MESSAGE_MAP(CTopWnd, CScrollWnd)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


void CTopWnd::OnDraw(CDC *pDC)
{
	CRect clr;
	GetClientRect(&clr);
	CRect cr = GetCanvasRect();

	CRect ur;
	ur.UnionRect(&cr, &clr);
	
	COLORREF bgcolor = pew->pCB->GetThemeColor("PE BG");
	COLORREF textcolor = pew->pCB->GetThemeColor("PE Text");

	pDC->FillSolidRect(&ur, bgcolor);

	CMachinePattern *ppat = pew->pPattern;

	if (ppat == NULL || ppat->columns.size() == 0)
		return;

	double dDpiScale = DPIScale(pDC);

	int topWndFontSize = INITIALX_96DPI_TOP_WND_FONT * dDpiScale;

	CFont fontTop;
	fontTop.CreatePointFont(INITIALX_96DPI_TOP_WND_FONT, "Segoe UI");


	LOGFONT lfont;
	fontTop.GetLogFont(&lfont);
	lfont.lfHeight = 100;
	lfont.lfWeight = FW_SEMIBOLD;
	lfont.lfQuality = PROOF_QUALITY;
	fontTop.DeleteObject();
	fontTop.CreatePointFontIndirect(&lfont);
	
	int topWndFontSize2 = INITIALX_96DPI_TOP_WND_FONT2 * dDpiScale;

	// Create Vertical font
	CFont myVerticalFont;
	int headerFontSize = 
	myVerticalFont.CreateFont(topWndFontSize2, 0, 900, 900, FALSE, FALSE, FALSE, 0, 0, 0,
		0, CLEARTYPE_QUALITY, 0, _T("Segoe UI"));
	
	// CObject *pOldFont = pDC->SelectObject(&pew->font);
	CObject* pOldFont = pDC->SelectObject(&fontTop);
	
	CPen pen(PS_SOLID, 1, Blend(textcolor, bgcolor, 0.5f));
	CObject *pOldPen = pDC->SelectObject(&pen);

	CColumn *lastcolumn = ppat->columns[0].get();
	int startx = 0;
	int macstartx = 0;
	int x = 0;

	for (int col = 0; col <= (int)ppat->columns.size(); col++)
	{
		CColumn *pc = NULL;
		if (col < (int)ppat->columns.size())
			pc = ppat->columns[col].get();

		if (pc == NULL || ((x - startx) > 0 && !pc->MatchGroupAndTrack(*lastcolumn)))
		{
			if (lastcolumn->IsTrackParam())
			{
				// draw track number
				if (pew->pPattern->IsTrackMuted(lastcolumn))
					pDC->SetTextColor(Blend(textcolor, bgcolor, 0.5f));
				else
					//pDC->SetTextColor(textcolor);
					pDC->SetTextColor(Blend(textcolor, bgcolor, 0.90f));

				CRect r;
				r.left = startx;
				r.right = x - pew->fontSize.cx;
				r.top = 2;
				r.bottom = r.top + 60 * dDpiScale;

				CString s;
				s.Format("%s | %s\nTrack %d", ppat->columns[col - 1]->GetMachineName(), ppat->name, lastcolumn->GetTrack());

				pDC->DrawText(s, &r, DT_TOP | DT_CENTER);

			}
			else
			{
				// draw track number
				if (pew->pPattern->IsTrackMuted(lastcolumn))
					pDC->SetTextColor(Blend(textcolor, bgcolor, 0.5f));
				else
					//pDC->SetTextColor(textcolor);
					pDC->SetTextColor(Blend(textcolor, bgcolor, 0.90f));

				CRect r;
				r.left = startx;
				r.right = x - pew->fontSize.cx;
				r.top = 2;
				r.bottom = r.top + 60 * dDpiScale;

				CString s;
				s.Format("%s | %s\nGlobal", ppat->columns[col - 1]->GetMachineName(), ppat->name);

				pDC->DrawText(s, &r, DT_TOP | DT_CENTER);
			}
 
			startx = x;
		}

		if (pc == NULL || !pc->MatchMachine(*lastcolumn))
		{
			/*
			// draw machine name
			pDC->SetTextColor(textcolor);

			//pDC->SelectObject(&fontTop);

			CRect r;
			r.left = macstartx;
			r.right = x - pew->fontSize.cx;
			if (r.right < 200)
				r.right = 200;
			r.top = 2;
			r.bottom = pew->fontSize.cy + 4;
			CString s;
			s.Format("%s | %s", ppat->columns[col - 1]->GetMachineName(), ppat->name);
			pDC->DrawText(s, &r, DT_CENTER | DT_WORD_ELLIPSIS);
			//pDC->DrawText(s, &r, DT_LEFT | DT_WORD_ELLIPSIS);

			pDC->SelectObject(&pew->font);
			*/
			if (pc != NULL)
			{
				COLORREF linecolor = Blend(textcolor, bgcolor, 0.2f);
				CPen penl(PS_SOLID, 1, linecolor);
				CObject* pOldFont_ = pDC->SelectObject(&penl);

				// draw a vertical separator between machines
				int sx = x - pew->fontSize.cx / 2;
				pDC->MoveTo(sx, 0);
				pDC->LineTo(sx, clr.bottom);

				pDC->SelectObject(&pOldFont_);
			}

			macstartx = x;
		}

		if (pc != NULL)
		{   // Draw param description
			// x : start of col
			
			::TEXTMETRIC metrics;
			pDC->GetTextMetrics(&metrics);

			lineHeight = metrics.tmHeight + metrics.tmExternalLeading;

			CRect r;
			r.left = x + (pew->pe.GetColumnWidth(col) - pew->fontSize.cx - lineHeight) / 2;
			r.right = x + pew->pe.GetColumnWidth(col);
			r.top = pew->topWndHeight - 96 * dDpiScale;
			r.bottom = pew->topWndHeight + 8 * dDpiScale;
			
			if (pew->pPattern->IsTrackMuted(pc))
				pDC->SetTextColor(Blend(textcolor, bgcolor, 0.5f));
			else
				pDC->SetTextColor(Blend(textcolor, bgcolor, 0.7f));

			// Use vertical font
			CFont* pOldFont_ = pDC->SelectObject(&myVerticalFont);

			CString s;
			s = pc->GetName();
			pDC->DrawText(s, &r, DT_BOTTOM | DT_SINGLELINE );

			// Restore prev font
			pDC->SelectObject(pOldFont_);
		}

		if (pc == NULL)
			break;

//		x += (pc->GetDigitCount() + 1) * pew->fontSize.cx;
		x += pew->pe.GetColumnWidth(col);
		
		lastcolumn = pc;
	}

	pDC->SetTextColor(textcolor);
	/*
	pDC->SelectObject(&fontTop);

	CRect re;
	re.left = 0;
	re.right = 200;
	re.top = 20;// pew->fontSize.cy;
	re.bottom = pew->fontSize.cy * 2;
	CString s;
	s.Format("Pattern: %s", ppat->name);
	pDC->DrawText(s, &re, DT_LEFT);
	*/

	COLORREF linecolor = Blend(textcolor, bgcolor, 0.2f);
	CPen penl(PS_SOLID, 1, linecolor);
	pDC->SelectObject(&penl);

	pDC->MoveTo(2, clr.bottom - 4);
	pDC->LineTo(ur.right - ur.left - 8, clr.bottom - 4);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

	//fontTop.DeleteObject();
	//myVerticalFont.DeleteObject();
}

void CTopWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	point = ClientToCanvas(point);
	if (point.y < lineHeight * 2 + 2)
	{
		int col = pew->pe.GetColumnAtX(point.x);
		if (col < 0)
			return;

		pew->pCB->ShowMachineWindow(pew->pPattern->columns[col]->GetMachine(), true);
		
	}
	else
	{
		//MuteTrack(point);
	}

	CScrollWnd::OnLButtonDblClk(nFlags, point);
}

void CTopWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	point = ClientToCanvas(point);
	if (point.y > lineHeight * 2 + 2)
	{
		CCursorPos mousePos = pew->pe.GetDigitAtPointIgnoreRow(point);
		pew->pe.MoveCursor(mousePos);
		pew->pe.ResetCursorTimer();
		return;
	}
	else if (point.y > lineHeight + 2)
	{
		MuteTrack(point);
	}

	CScrollWnd::OnLButtonDown(nFlags, point);
}

void CTopWnd::MuteTrack(CPoint point)
{
	int col = pew->pe.GetColumnAtX(point.x);
	if (col < 0)
		return;

	CColumn *pc = pew->pPattern->columns[col].get();
	if (pc->IsTrackParam())
		pew->pPattern->ToggleTrackMute(pc);

	Invalidate();
	pew->pe.Invalidate();

}

int CTopWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	unsigned int uid = 0;       // for ti initialization
	hwndTT = CreateWindowEx(WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		NULL,
		TTS_NOPREFIX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL
	);

	// INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
	//ti.cbSize = sizeof(TOOLINFO);
	ti.cbSize = TTTOOLINFO_V1_SIZE;
	ti.uFlags = TTF_TRACK;
	ti.hwnd = NULL;
	ti.hinst = NULL;
	ti.uId = uid;
	ti.lpszText = _T("");
	// ToolTip control will cover the whole window
	ti.rect.left = 0;
	ti.rect.top = 0;
	ti.rect.right = 0;
	ti.rect.bottom = 0;
	::SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

	return 0;
}

void CTopWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollWnd::OnMouseMove(nFlags, point);

	if (!toolTipEnalbed || (lastTTPoint == point))
		return;

	lastTTPoint = point;

	CMachinePattern* ppat = pew->pPattern;

	if (ppat == NULL || ppat->columns.size() == 0)
		return;
	
	CPoint oldPoint = point;
	ClientToScreen(&oldPoint);
	CRect clr;
	GetClientRect(&clr);

	point = ClientToCanvas(point);
	if (point.y > lineHeight * 2 + 2 && point.y < pew->topWndHeight + 4)
	{
		CCursorPos mousePos = pew->pe.GetDigitAtPointIgnoreRow(point);

		int col = pew->pe.GetColumnAtX(point.x);
		if (col < 0)
			return;

		double dDpiScale = DPIScale(GetDC());

		CColumn* pc = ppat->columns[col].get();

		ti.lpszText = (LPSTR)(LPCSTR)pc->GetName();
		::SendMessage(hwndTT, TTM_SETTOOLINFO, 0, (LPARAM)&ti);
		::SendMessage(hwndTT, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD)MAKELONG(oldPoint.x - 30 * dDpiScale, oldPoint.y - 30 * dDpiScale));
		::SendMessage(hwndTT, TTM_TRACKACTIVATE, true, (LPARAM)(LPTOOLINFO)&ti);
		
	}
	else
	{
		::SendMessage(hwndTT, TTM_TRACKACTIVATE, false, (LPARAM)(LPTOOLINFO)&ti);
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);
}

void CTopWnd::OnMouseLeave()
{
	::SendMessage(hwndTT, TTM_TRACKACTIVATE, false, (LPARAM)(LPTOOLINFO)&ti);

	CScrollWnd::OnMouseLeave();
}