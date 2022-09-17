#pragma once

#include "CursorPos.h"

typedef vector<byte> ByteVector;

class CState
{
public:
	CString name;
	ByteVector state;
	int patternLength;
	CCursorPos cursorpos;
	CPoint scrollpos;
};

class CMemDataOutput : public CMachineDataOutput
{
public:
	CMemDataOutput(ByteVector* p) { pdata = p; }

	virtual void Write(void* pbuf, int const numbytes)
	{
		int oldsize = (int)pdata->size();
		pdata->resize(oldsize + numbytes);
		memcpy(&(*pdata)[oldsize], pbuf, numbytes);		// try to avoid: &pdata[oldsize]
	}

	ByteVector* pdata;
};

class CMemDataInput : public CMachineDataInput
{
public:
	CMemDataInput(ByteVector* p) { pdata = p; pos = 0; }

	virtual void Read(void* pbuf, int const numbytes)
	{
		ASSERT(pos + numbytes <= (int)pdata->size());
		memcpy(pbuf, &(*pdata)[pos], numbytes);
		pos += numbytes;
	}

	ByteVector* pdata;
	int pos;
};

typedef vector<shared_ptr<CState>> StateVector;

class CEditorWnd;

class CActionStack
{
public:
	CActionStack();
	~CActionStack();

	void BeginAction(CEditorWnd *pew, char const *name);

	void Undo(CEditorWnd *pew);
	void Redo(CEditorWnd *pew);

	bool CanUndo() const { return position > 0; }
	bool CanRedo() const { return position < (int)states.size(); }

private:
	void RestoreState(CEditorWnd *pew);
	void SaveState(CEditorWnd *pew, CState &s);

private:
	int position;
	StateVector states;
	CState unmodifiedState;

};
