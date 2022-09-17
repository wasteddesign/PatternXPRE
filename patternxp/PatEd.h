#pragma once

#include "MachinePattern.h"
#include "ScrollWnd.h"
#include "CursorPos.h"

#define PLAY_POS_TIMER 1
#define CURSOR_TIMER 2
#define FOLLOW_PLAY_POS_JUMP_SIZE 4

#define PATTERN_BOX_CONTROL_ID 103

class CEditorWnd;

enum ECursorType
{
	Blinking,
	NonBlinking
};

// CPatEd

class CPatEd : public CScrollWnd
{
	DECLARE_DYNCREATE(CPatEd)

public:
	static int const WM_MIDI_NOTE = WM_USER + 1;
	static const int NumNoteCols = 12;
	static const int NumNoteColorPalettes = 6;

	COLORREF curcol;

public:
	CPatEd();
	virtual ~CPatEd();

	void Create(CWnd *parent);
	void SetPattern(CMachinePattern *p);

	void SetPlayPos(MapIntToPlayingPattern &pp, CMasterInfo *pmi);

	virtual void OnDraw(CDC *pDC);

	void PatternChanged();
	void ColumnsChanged();

	CColumn *GetCursorColumn();

	void OnEditCut();
	void OnEditCopy();
	void OnEditPaste();

	bool CanCut();
	bool CanCopy();
	bool CanPaste();

	int GetColumnAtX(int x);
	
	void InvalidateInTimer() { invalidateInTimer = true; }

	int GetColumnWidth(int column);

	CRect GetCursorRect();
	CCursorPos GetDigitAtPointIgnoreRow(CPoint p);
	void MoveCursor(CCursorPos newpos, bool killsel = true);
	void ResetCursorTimer();

private:
	void DrawColumn(CDC *pDC, int col, int x, COLORREF textcolor, CRect const &clipbox);
	void DrawField(CDC *pDC, int col, CColumn *pnc, int data, int x, int y, bool muted, bool hasvalue);
	void DrawGraphicalField(CDC *pDC, int col, CColumn *pnc, int data, int x, int y, bool muted, bool hasvalue, COLORREF textcolor);
	void DrawCursor(CDC *pDC);
	int GetColumnX(int column);
	CRect GetCursorRect(CCursorPos const &p);
	
	void MoveCursorDelta(int dx, int dy);
	void Tab();
	void ShiftTab();
	int GetDigitAtX(int x);
	int GetRowY(int y);
	void UpdateStatusBar();
	void InvalidateField(int row, int column);
	void InvalidateGroup(int row, int column);
	void EditNote(int note, bool canplay = true);
	void EditOctave(int oct);
	void EditByte(int n);
	void EditWord(int n);
	void EditSwitch(int n);
	void EditAscii(char n);
	void Clear();
	void Insert();
	void Delete();
	void Home();
	void End();
	CRect GetSelRect();
	CRect GetSelOrCursorRect();
	CRect GetSelOrAll();
	void KillSelection();
	void CursorSelect(int dx, int dy);
	bool InSelection(int row, int column);
	void Randomize();
	void Interpolate(bool expintp);
	void ShiftValues(int delta);
	void WriteState();
	void MuteTrack();
	CCursorPos GetDigitAtPoint(CPoint p);
	void PlayRow(bool allcolumns);
	void OldSelect(bool start);
	void Rotate(bool reverse);
	void ToggleGraphicalMode();
	COLORREF GetFieldBackgroundColor(CMachinePattern *ppat, int row, int col, bool muted);
	void Draw(CPoint point);
	void ImportOld();

	// ToDo: these will copy machine data to clipboard so that some other PXP can import the data
	void CopyToClipboard();
	void CopyFromClipboard();

	void DrawSelection(CDC* pDC);
	void SetDrawShadow(bool ds, bool save);

	COLORREF bgcol;
	COLORREF bgcoldark;
	COLORREF bgcolvdark;
	COLORREF bgsel;
	COLORREF textcolor;
	COLORREF shadowcolor;
	//COLORREF dotcol;
	
	ECursorType cursorType;
	bool blinkingCursorVisible;
	bool windowFocused;

	COLORREF notecols[NumNoteColorPalettes * NumNoteCols] = { 0x4C4CF5, 0xA1A130, 0xB754FF, 0x3AC03A, 0xF158AD, 0x57CFF6, 0xDC752D, 0x8888FF, 0x74B335, 0xF053F0, 0x58DC9A, 0xFEAC6C,
	                                       0x2C2CD5, 0x818110, 0x9734DF, 0x1AA01A, 0xD1388D, 0x37AFD6, 0xBC550D, 0x6868DF, 0x549315, 0xD033D0, 0x38BC7A, 0xDE8C4C,
										   0x0C0CB5, 0x616100, 0x7714BF, 0x0A900A, 0xB1186D, 0x178FB6, 0x9C350D, 0x4848BF, 0x347305, 0xB013B0, 0x189C5A, 0xBE6C2C,
	                                       0x00F522, 0x70F592, 0x10F532, 0x80F5A2, 0x20F542, 0x30F552, 0x90F5B2, 0x40F562, 0xA0F5C2, 0x50F572, 0xB0F5D2, 0x60F582,
										   0xEE4C20, 0xFEBC90, 0xFE5C30, 0xFECCA0, 0xFE6C40, 0xFE7C50, 0xFEDCB0, 0xFE8C60, 0xFEECC0, 0xFE9C70, 0xFEFCD0, 0xFEAC80,
                                           0x0909F9, 0x7979F9, 0x1919F9, 0x8989F9, 0x2929F9, 0x3939F9, 0x9999F9, 0x4949F9, 0xA9A9F9, 0x5959F9, 0xB9B9F9, 0x6969F9 };
	int colorPalette = 1;

	int mouseWheelAcc;
	int cursorStep;

	int targetRowPrev = 0;

	bool selection;
	CPoint selStart;
	CPoint selEnd;

	CMachinePattern *pPlayingPattern;
	int playPos;
	int drawPlayPos;

	CCursorPos mouseSelectStartPos;
	bool mouseSelecting;
	bool persistentSelection;

	enum SelectionMode { column, track, group, all };
	SelectionMode selMode;

	bool invalidateInTimer;

	bool drawing;
	int drawColumn;

	static MapIntToIntVector clipboard;
	static int clipboardRowCount;
	static bool clipboardPersistentSelection;
	static SelectionMode clipboardSelMode;

	bool drawShadow;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CCursorPos cursor;

	CEditorWnd *pew;
	CMICallbacks *pCB;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pWnd);

	afx_msg LRESULT OnMidiNote(WPARAM wParam, LPARAM lParam);

};


