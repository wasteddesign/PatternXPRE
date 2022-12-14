#pragma once

#include "ScrollWnd.h"

class CEditorWnd;

// CTopWnd

class CTopWnd : public CScrollWnd
{
	DECLARE_DYNAMIC(CTopWnd)

public:
	CTopWnd();
	virtual ~CTopWnd();

	virtual void OnDraw(CDC *pDC);

	CEditorWnd *pew;

private:
	void MuteTrack(CPoint point);
	int lineHeight;
	HWND hwndTT;
	TOOLINFO ti;
	bool toolTipEnalbed;

	CPoint lastTTPoint;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


