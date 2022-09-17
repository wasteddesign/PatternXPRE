#pragma once

#define EDITOR_MAIN_TOOLBAR 105

#include "MachinePattern.h"
#include "PatEd.h"
#include "EmptyWnd.h"
#include "RowNumWnd.h"
#include "TopWnd.h"
#include "FuBar.h"
#include "ToolBar2.h"
#include "ColumnDialog.h"
#include "ActionStack.h"

class CEditorWnd : public CWnd
{
	DECLARE_DYNAMIC(CEditorWnd)

public:
	CEditorWnd();
	virtual ~CEditorWnd();

	void Create(HWND parent);
	void SetPattern(CMachinePattern *p);

	void SetPatternLength(CMachinePattern *p, int length);
	void SetPlayPos(MapIntToPlayingPattern &pp, CMasterInfo *pmi);

	void AddTrack(int n = 1);
	void DeleteLastTrack();

	bool EnableCommandUI(int id);

	void UpdateCanvasSize();

	int GetEditorPatternPosition();
	void UpdateWindowSizes();

	void SetFollowPlayPos(bool follow, bool save = false);
	void SetFollowPlayingPattern(bool follow, bool save = false);

private:	
	
	void FontChanged();

	
protected:
	DECLARE_MESSAGE_MAP()
			
public:
	CMachineInterfaceEx* ex;
	CMachineInterface* mi;
	CMICallbacks *pCB;
	CMachine *pMachine;
	CGlobalData *pGlobalData;
//	CParamWnd pw;
	CPatEd pe;
	CRowNumWnd leftwnd;
	CTopWnd topwnd;
	CEmptyWnd topleftwnd;

	CMachinePattern *pPattern;
	CFont font;
	CSize fontSize;
	int rowNumWndWidth;
	int topWndHeight;

	CFuBar reBar;
	CToolBar2 toolBar;
	CDialogBar dlgBar;

	bool MidiEditMode;
	bool dialogOpen;

	ULONG_PTR gdiplusToken;

	bool followPlayPos;
	bool followPattern;

	void ToggleMidiEdit();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pWnd);
	afx_msg void OnColumns();
	afx_msg void OnSelectFont();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);

	afx_msg void OnUpdateClipboard(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();

protected:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedColumnsButton();
	afx_msg void OnBnClickedFontButton();
	afx_msg void OnBnClickedMidiEdit();
	afx_msg void OnBnClickedFollowPlayPos();
	afx_msg void OnBnClickedFollowPlayingPattern();
};


